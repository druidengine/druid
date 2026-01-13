module;

#include <include/core/SkCanvas.h>
#include <include/core/SkFont.h>
#include <include/core/SkImageInfo.h>
#include <include/core/SkPaint.h>
#include <include/core/SkRect.h>
#include <include/core/SkSurface.h>
#include <include/core/SkTextBlob.h>
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
		{
			const SkImageInfo info = SkImageInfo::MakeN32Premul(width, height);
			surface_ = SkSurfaces::Raster(info);

			if (surface_)
			{
				canvas_ = surface_->getCanvas();
			}
		}

		/// @brief Virtual destructor.
		~Renderer() override = default;

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
			if (!canvas_)
			{
				return;
			}

			const SkColor sk_color = SkColorSetARGB(clear.alpha(), clear.red(), clear.green(), clear.blue());
			canvas_->clear(sk_color);
		}

		/// @brief End the current rendering frame.
		///
		/// Flushes the Skia surface to finalize drawing operations.
		auto end() -> void override
		{
			if (surface_)
			{
				surface_->flushAndSubmit();
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

			canvas_->drawSimpleText(text.c_str(), text.size(), SkTextEncoding::kUTF8, x, y, font, paint);
		}

	private:
		sk_sp<SkSurface> surface_;
		SkCanvas* canvas_{nullptr};
	};
}
