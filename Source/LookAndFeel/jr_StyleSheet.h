#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "Resources/FontResources.h"

namespace jr
{
    // fonts
    namespace StyleSheet
    {

        static juce::Typeface::Ptr boldFont  { juce::Typeface::createSystemTypefaceFor (Resources::WorkSansSemiBold_ttf, Resources::WorkSansSemiBold_ttfSize) };
        static juce::Typeface::Ptr plainFont { juce::Typeface::createSystemTypefaceFor (Resources::WorkSansRegular_ttf, Resources::WorkSansRegular_ttfSize) };

    }

    class CustomLookAndFeel : public juce::LookAndFeel_V4
    {
    public:
        CustomLookAndFeel()
        {
            setDefaultSansSerifTypeface(StyleSheet::boldFont);
            setColour(juce::TextButton::textColourOffId, juce::Colours::black);
            setColour(juce::ComboBox::textColourId, juce::Colours::black);
            setColour(juce::ToggleButton::textColourId, juce::Colours::black);
            setColour(juce::Label::textColourId, juce::Colours::black);
            setColour(juce::Label::textWhenEditingColourId, juce::Colours::black);
            setColour(juce::Label::outlineWhenEditingColourId, juce::Colours::black);
            setColour(juce::Label::backgroundWhenEditingColourId, juce::Colours::white);
            setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
            setColour(juce::TextEditor::textColourId, juce::Colours::black);
            setColour(juce::TextEditor::highlightedTextColourId, juce::Colours::black);
            setColour(juce::ToggleButton::tickColourId, juce::Colours::black);
            setColour(juce::ToggleButton::textColourId, juce::Colours::black);
        }

        void drawButtonBackground(juce::Graphics& g,
            juce::Button& button,
            const juce::Colour& backgroundColour,
            bool shouldDrawButtonAsHighlighted,
            bool shouldDrawButtonAsDown) override
        {
            auto mainColour = juce::Colours::white;

            auto cornerSize = 0.0f;
            auto bounds = button.getLocalBounds().toFloat().reduced(0.5f, 0.5f);

            auto baseColour = mainColour.withMultipliedSaturation(button.hasKeyboardFocus(true) ? 1.3f : 0.9f)
                .withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f);

            if (shouldDrawButtonAsDown || shouldDrawButtonAsHighlighted)
                baseColour = baseColour.contrasting(shouldDrawButtonAsDown ? 0.2f : 0.05f);

            juce::Colour borderColour = juce::Colours::black;

            auto flatOnLeft = button.isConnectedOnLeft();
            auto flatOnRight = button.isConnectedOnRight();
            auto flatOnTop = button.isConnectedOnTop();
            auto flatOnBottom = button.isConnectedOnBottom();

            if (flatOnLeft || flatOnRight || flatOnTop || flatOnBottom)
            {
                juce::Path path;
                path.addRoundedRectangle(bounds.getX(), bounds.getY(),
                    bounds.getWidth(), bounds.getHeight(),
                    cornerSize, cornerSize,
                    !(flatOnLeft || flatOnTop),
                    !(flatOnRight || flatOnTop),
                    !(flatOnLeft || flatOnBottom),
                    !(flatOnRight || flatOnBottom));

                g.fillPath(path);
            }
            else
            {
                g.setColour(baseColour);
                g.fillRoundedRectangle(bounds, cornerSize);
                g.setColour(borderColour);
                g.drawRect(bounds, 2.0f);
            }
        }

        void drawComboBox(juce::Graphics& g, int width, int height, bool,
            int, int, int, int, juce::ComboBox& box) override
        {
            juce::Rectangle<int> boxBounds(0, 0, width, height);

            g.setColour(juce::Colours::white);
            g.fillRect(boxBounds.toFloat());
            g.setColour(juce::Colours::black);
            g.drawRect(boxBounds.toFloat(), 2.0f);

            juce::Rectangle<int> arrowZone(width - 30, 0, 20, height);
            juce::Path path;
            path.startNewSubPath((float)arrowZone.getX() + 3.0f, (float)arrowZone.getCentreY() - 2.0f);
            path.lineTo((float)arrowZone.getCentreX(), (float)arrowZone.getCentreY() + 3.0f);
            path.lineTo((float)arrowZone.getRight() - 3.0f, (float)arrowZone.getCentreY() - 2.0f);

            g.setColour(juce::Colours::black.withAlpha((box.isEnabled() ? 0.9f : 0.2f)));
            g.strokePath(path, juce::PathStrokeType(2.0f));
        }

        void drawPopupMenuBackground(juce::Graphics& g, [[maybe_unused]] int width, [[maybe_unused]] int height) override
        {
            g.fillAll(juce::Colours::white);
        }

        void drawPopupMenuItem(juce::Graphics& g, const juce::Rectangle<int>& area,
            const bool isSeparator, const bool isActive,
            const bool isHighlighted, const bool isTicked,
            const bool hasSubMenu, const juce::String& text,
            const juce::String& shortcutKeyText,
            const juce::Drawable* icon, const juce::Colour* const textColourToUse) override
        {
            if (isSeparator)
            {
                auto r = area.reduced(5, 0);
                r.removeFromTop(juce::roundToInt(((float)r.getHeight() * 0.5f) - 0.5f));

                g.setColour(findColour(juce::PopupMenu::textColourId).withAlpha(0.3f));
                g.fillRect(r.removeFromTop(1));
            }
            else
            {
                auto textColour = juce::Colours::black;

                auto r = area.reduced(1);

                if (isHighlighted && isActive)
                {
                    g.setColour(juce::Colours::lightgrey);
                    g.fillRect(r);

                    g.setColour(juce::Colours::black);
                }
                else
                {
                    g.setColour(textColour.withMultipliedAlpha(isActive ? 1.0f : 0.5f));
                }

                r.reduce(juce::jmin(5, area.getWidth() / 20), 0);

                auto font = getPopupMenuFont();

                auto maxFontHeight = (float)r.getHeight() / 1.3f;

                if (font.getHeight() > maxFontHeight)
                    font.setHeight(maxFontHeight);

                g.setFont(font);

                auto iconArea = r.removeFromLeft(juce::roundToInt(maxFontHeight)).toFloat();

                if (icon != nullptr)
                {
                    icon->drawWithin(g, iconArea, juce::RectanglePlacement::centred | juce::RectanglePlacement::onlyReduceInSize, 1.0f);
                    r.removeFromLeft(juce::roundToInt(maxFontHeight * 0.5f));
                }
                else if (isTicked)
                {
                    auto tick = getTickShape(1.0f);
                    g.fillPath(tick, tick.getTransformToScaleToFit(iconArea.reduced(iconArea.getWidth() / 5, 0).toFloat(), true));
                }

                if (hasSubMenu)
                {
                    auto arrowH = 0.6f * getPopupMenuFont().getAscent();

                    auto x = static_cast<float> (r.removeFromRight((int)arrowH).getX());
                    auto halfH = static_cast<float> (r.getCentreY());

                    juce::Path path;
                    path.startNewSubPath(x, halfH - arrowH * 0.5f);
                    path.lineTo(x + arrowH * 0.6f, halfH);
                    path.lineTo(x, halfH + arrowH * 0.5f);

                    g.strokePath(path, juce::PathStrokeType(2.0f));
                }

                r.removeFromRight(3);
                g.drawFittedText(text, r, juce::Justification::centredLeft, 1);

                if (shortcutKeyText.isNotEmpty())
                {
                    auto f2 = font;
                    f2.setHeight(f2.getHeight() * 0.75f);
                    f2.setHorizontalScale(0.95f);
                    g.setFont(f2);

                    g.drawText(shortcutKeyText, r, juce::Justification::centredRight, true);
                }
            }
        }

        void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
            float sliderPos, float minSliderPos, float maxSliderPos,
            juce::Slider::SliderStyle style, juce::Slider& slider) override {
            if (slider.isBar())
            {
               // not implemented
            }
            else
            {
                auto isTwoVal = (style == juce::Slider::SliderStyle::TwoValueVertical || style == juce::Slider::SliderStyle::TwoValueHorizontal);
                auto isThreeVal = (style == juce::Slider::SliderStyle::ThreeValueVertical || style == juce::Slider::SliderStyle::ThreeValueHorizontal);

                auto const trackWidth = 4.0f;

                juce::Point<float> startPoint(slider.isHorizontal() ? (float)x : (float)x + (float)width * 0.5f,
                    slider.isHorizontal() ? (float)y + (float)height * 0.5f : (float)(height + y));

                juce::Point<float> endPoint(slider.isHorizontal() ? (float)(width + x) : startPoint.x,
                    slider.isHorizontal() ? startPoint.y : (float)y);

                juce::Path backgroundTrack;
                backgroundTrack.startNewSubPath(startPoint);
                backgroundTrack.lineTo(endPoint);
                g.setColour(slider.findColour(juce::Slider::backgroundColourId));
                g.strokePath(backgroundTrack, { trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::butt });

                juce::Path valueTrack;
                juce::Point<float> minPoint, maxPoint, thumbPoint;

                if (isTwoVal || isThreeVal)
                {
                    minPoint = { slider.isHorizontal() ? minSliderPos : (float)width * 0.5f,
                                 slider.isHorizontal() ? (float)height * 0.5f : minSliderPos };

                    if (isThreeVal)
                        thumbPoint = { slider.isHorizontal() ? sliderPos : (float)width * 0.5f,
                                       slider.isHorizontal() ? (float)height * 0.5f : sliderPos };

                    maxPoint = { slider.isHorizontal() ? maxSliderPos : (float)width * 0.5f,
                                 slider.isHorizontal() ? (float)height * 0.5f : maxSliderPos };
                }
                else
                {
                    auto kx = slider.isHorizontal() ? sliderPos : ((float)x + (float)width * 0.5f);
                    auto ky = slider.isHorizontal() ? ((float)y + (float)height * 0.5f) : sliderPos;

                    minPoint = startPoint;
                    maxPoint = { kx, ky };
                }

                auto thumbWidth = 12.0f;

                valueTrack.startNewSubPath(minPoint);
                valueTrack.lineTo(isThreeVal ? thumbPoint : maxPoint);
                g.setColour(slider.findColour(juce::Slider::trackColourId));
                g.strokePath(valueTrack, { trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::butt });

                if (!isTwoVal)
                {
                    auto valueMarkerRect = juce::Rectangle<float>(thumbWidth, thumbWidth).withCentre(isThreeVal ? thumbPoint : maxPoint);
                    g.setColour(juce::Colours::white);
                    g.fillRect(valueMarkerRect);
                    g.setColour(juce::Colours::black);
                    g.drawRect(valueMarkerRect, 2.0f);
                }

                if (isTwoVal || isThreeVal)
                {
                    auto sr = juce::jmin(trackWidth, (slider.isHorizontal() ? (float)height : (float)width) * 0.4f);
                    auto pointerColour = slider.findColour(juce::Slider::thumbColourId);

                    if (slider.isHorizontal())
                    {
                        drawPointer(g, minSliderPos - sr,
                            juce::jmax(0.0f, (float)y + (float)height * 0.5f - trackWidth * 2.0f),
                            trackWidth * 2.0f, pointerColour, 2);

                        drawPointer(g, maxSliderPos - trackWidth,
                            juce::jmin((float)(y + height) - trackWidth * 2.0f, (float)y + (float)height * 0.5f),
                            trackWidth * 2.0f, pointerColour, 4);
                    }
                    else
                    {
                        drawPointer(g, juce::jmax(0.0f, (float)x + (float)width * 0.5f - trackWidth * 2.0f),
                            minSliderPos - trackWidth,
                            trackWidth * 2.0f, pointerColour, 1);

                        drawPointer(g, juce::jmin((float)(x + width) - trackWidth * 2.0f, (float)x + (float)width * 0.5f), maxSliderPos - sr,
                            trackWidth * 2.0f, pointerColour, 3);
                    }
                }
            }
        }

        void drawTickBox(juce::Graphics& g, juce::Component& component,
            float x, float y, float w, float h,
            const bool ticked,
            [[maybe_unused]] const bool isEnabled,
            [[maybe_unused]] const bool shouldDrawButtonAsHighlighted,
            [[maybe_unused]] const bool shouldDrawButtonAsDown)
        {
            juce::Rectangle<float> tickBounds(x, y, w, h);

            g.setColour(component.findColour(juce::ToggleButton::tickColourId));
            g.drawRect(tickBounds, 2.0f);

            if (ticked)
            {
                g.setColour(component.findColour(juce::ToggleButton::tickColourId));
                auto tick = getTickShape(0.75f);
                g.fillPath(tick, tick.getTransformToScaleToFit(tickBounds.reduced(4, 5).toFloat(), false));
            }
        }

        void drawLabel(juce::Graphics& g, juce::Label& label) override
        {
            g.fillAll(label.findColour(juce::Label::backgroundColourId));

            if (!label.isBeingEdited())
            {
                auto alpha = label.isEnabled() ? 1.0f : 0.5f;
                const juce::Font font(getLabelFont(label));

                g.setColour(juce::Colours::black);
                g.setFont(font);

                auto textArea = getLabelBorderSize(label).subtractedFrom(label.getLocalBounds());

                g.drawFittedText(label.getText(), textArea, label.getJustificationType(),
                    juce::jmax(1, (int)((float)textArea.getHeight() / font.getHeight())),
                    label.getMinimumHorizontalScale());
            }
        }

    private:
        juce::Colour buttonColour{ juce::Colour::fromRGB(176, 118, 153) };
        juce::Colour darkerButtonColour{ juce::Colour::fromRGB(136, 116, 152) };
    };
}
