module;

#include <include/core/SkCanvas.h>
#include <include/core/SkColorSpace.h>
#include <include/core/SkFont.h>
#include <include/core/SkFontMgr.h>
#include <include/core/SkFontStyle.h>
#include <include/core/SkImageInfo.h>
#include <include/core/SkPaint.h>
#include <include/core/SkRect.h>
#include <include/core/SkSurface.h>
#include <include/core/SkTextBlob.h>
#include <include/core/SkTypeface.h>
#include <include/gpu/ganesh/GrBackendSurface.h>
#include <include/gpu/ganesh/GrDirectContext.h>
#include <include/gpu/ganesh/SkSurfaceGanesh.h>
#include <include/gpu/ganesh/gl/GrGLBackendSurface.h>
#include <include/gpu/ganesh/gl/GrGLDirectContext.h>
#include <include/gpu/ganesh/gl/GrGLInterface.h>

// Platform-specific font managers
#ifdef _WIN32
#include <include/ports/SkTypeface_win.h>
#elif defined(__APPLE__)
#include <include/ports/SkFontMgr_mac_ct.h>
#elif defined(__ANDROID__)
#include <include/ports/SkFontMgr_android.h>
#elif defined(__EMSCRIPTEN__)
#include <include/ports/SkFontMgr_data.h>
#else
#include <include/ports/SkFontMgr_fontconfig.h>
#endif

#include <iostream>
#include <memory>
#include <string>

export module druid.skia.Renderer;

import druid.graphics.Color;
import druid.graphics.Renderer;

export namespace druid::skia
{
	/// @class Renderer
	/// @brief Skia-backed implementation of the Renderer interface.
	///
	/// This class adapts the abstract `druid::graphics::Renderer` API to the
	/// Skia graphics library. It translates engine-agnostic draw commands
	/// into Skia canvas operations.
	///
	/// `Renderer` owns the SkSurface and SkCanvas for rendering.
	class Renderer : public druid::graphics::Renderer
	{
	public:
		/// @brief Construct renderer with specified surface dimensions.
		/// @param width Surface width in pixels.
		/// @param height Surface height in pixels.
		explicit Renderer(int width, int height)
			: width_(width), height_(height)
		{
			std::cout << "Skia Renderer: Initializing with dimensions " << width << "x" << height << "\n";

			// Create OpenGL interface
			sk_sp<const GrGLInterface> interface = GrGLMakeNativeInterface();
			if (!interface)
			{
				std::cerr << "Skia Renderer: Failed to create GrGLInterface\n";
				return;
			}
			std::cout << "Skia Renderer: Created GL interface\n";

			// Create GPU direct context
			context_ = GrDirectContexts::MakeGL(interface);
			if (!context_)
			{
				std::cerr << "Skia Renderer: Failed to create GrDirectContext\n";
				return;
			}
			std::cout << "Skia Renderer: Created GPU context\n";

			// Create platform-specific font manager
#ifdef _WIN32
			std::cout << "Skia Renderer: Using DirectWrite font manager (Windows)\n";
			fontMgr_ = SkFontMgr_New_DirectWrite();
#elif defined(__APPLE__)
			std::cout << "Skia Renderer: Using CoreText font manager (macOS/iOS)\n";
			fontMgr_ = SkFontMgr_New_CoreText(nullptr);
#elif defined(__ANDROID__)
			std::cout << "Skia Renderer: Using Android font manager\n";
			fontMgr_ = SkFontMgr_New_Android(nullptr);
#elif defined(__EMSCRIPTEN__)
			std::cout << "Skia Renderer: Using data font manager (WebAssembly)\n";
			fontMgr_ = SkFontMgr_New_Custom_Data();
#else
			std::cout << "Skia Renderer: Using FontConfig font manager (Linux)\n";
			fontMgr_ = SkFontMgr_New_FontConfig(nullptr);
#endif

			if (fontMgr_)
			{
				typeface_ = fontMgr_->legacyMakeTypeface(nullptr, SkFontStyle());
				if (typeface_)
				{
					std::cout << "Skia Renderer: Created default typeface\n";
				}
				else
				{
					std::cerr << "Skia Renderer: Failed to create typeface\n";
				}
			}
			else
			{
				std::cerr << "Skia Renderer: Failed to get font manager\n";
			}
		}

		/// @brief Virtual destructor.
		~Renderer() override
		{
			// Release surface first
			surface_.reset();
			
			// Abandon context instead of flushing to prevent hang
			if (context_)
			{
				context_->abandonContext();
				context_.reset();
			}
		}

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept = delete;
		auto operator=(const Renderer&) -> Renderer& = delete;
		auto operator=(Renderer&&) noexcept -> Renderer& = delete;

		/// @brief Get the Skia surface.
		/// @return Pointer to the owned SkSurface.
		[[nodiscard]] auto get_surface() -> SkSurface*
		{
			return surface_.get();
		}

		/// @brief Begin a new rendering frame.
		///
		/// Clears the canvas to the specified background color.
		///
		/// @param clear Background clear color.
		auto begin(druid::graphics::Color clear) -> void override
		{
			if (!context_)
			{
				return;
			}

			// Create or recreate surface wrapping the window's framebuffer
			if (!surface_)
			{
				// Get framebuffer info - assuming default framebuffer 0
				GrGLFramebufferInfo fbInfo;
				fbInfo.fFBOID = 0; // Default framebuffer
				fbInfo.fFormat = 0x8058; // GL_RGBA8

				// Use the GL-specific helper to create backend render target
				GrBackendRenderTarget backendRT = GrBackendRenderTargets::MakeGL(width_, height_, 0, 0, fbInfo);

				// Create surface that wraps the framebuffer
				surface_ = SkSurfaces::WrapBackendRenderTarget(
					context_.get(),
					backendRT,
					kBottomLeft_GrSurfaceOrigin,
					kRGBA_8888_SkColorType,
					nullptr,
					nullptr
				);

				if (surface_)
				{
					canvas_ = surface_->getCanvas();
					std::cout << "Skia Renderer: Created surface wrapping framebuffer\n";
				}
				else
				{
					std::cerr << "Skia Renderer: Failed to wrap framebuffer\n";
					return;
				}
			}

			if (canvas_)
			{
				const SkColor sk_color = SkColorSetARGB(clear.alpha(), clear.red(), clear.green(), clear.blue());
				canvas_->clear(sk_color);
			}
		}

		/// @brief End the current rendering frame.
		///
		/// Flushes the GPU context to finalize drawing operations.
		auto end() -> void override
		{
			if (context_)
			{
				context_->flushAndSubmit();
			}
		}

		/// @brief Draw a filled rectangle.
		///
		/// @param x X-coordinate of the rectangle origin.
		/// @param y Y-coordinate of the rectangle origin.
		/// @param width Rectangle width.
		/// @param height Rectangle height.
		/// @param color Fill color.
		auto draw_rectangle(float x, float y, float width, float height, druid::graphics::Color color) -> void override
		{
			if (!canvas_)
			{
				return;
			}

			SkPaint paint;
			paint.setColor(SkColorSetARGB(color.alpha(), color.red(), color.green(), color.blue()));
			paint.setStyle(SkPaint::kFill_Style);

			const SkRect rect = SkRect::MakeXYWH(x, y, width, height);
			canvas_->drawRect(rect, paint);
		}

		/// @brief Draw a text string.
		///
		/// @param x X-coordinate of the text origin.
		/// @param y Y-coordinate of the text baseline.
		/// @param text UTF-8 text string to render.
		/// @param font_size Font size in pixels.
		/// @param color Text color.
		auto draw_text(float x, float y, const std::string& text, int font_size, druid::graphics::Color color) -> void override
		{
			if (!canvas_)
			{
				return;
			}

			SkPaint paint;
			paint.setColor(SkColorSetARGB(color.alpha(), color.red(), color.green(), color.blue()));
			paint.setAntiAlias(true);

			SkFont font;
			font.setSize(static_cast<SkScalar>(font_size));
			if (typeface_)
			{
				font.setTypeface(typeface_);
			}

			canvas_->drawSimpleText(text.c_str(), text.size(), SkTextEncoding::kUTF8, x, y, font, paint);
		}

	private:
		sk_sp<GrDirectContext> context_;
		sk_sp<SkSurface> surface_;
		SkCanvas* canvas_{nullptr};
		sk_sp<SkFontMgr> fontMgr_;
		sk_sp<SkTypeface> typeface_;
		int width_{0};
		int height_{0};
	};
}
