#pragma once

#include "View.h"
#include <vector>
#include <SFML/Graphics.hpp>

class AnimatedSprite : public View
{
public:
    AnimatedSprite()
        : mCurrentFrame(0), mAnimationTime(0.f), mFrameTime(0.7f)
    {
        mBackground.setFillColor(sf::Color::Transparent);
    }

    sf::Vector2f GetCenter() const override
    {
        return mBackground.getPosition() + mBackground.getSize() / 2.f;
    }

    sf::Vector2f GetSize() const override
    {
        return mBackground.getSize();
    }

    AnimatedSprite& SetTextures(const std::vector<sf::Texture>& textures)
    {
        mTextures = textures;
        if (!mTextures.empty())
        {
            mSprite.setTexture(mTextures[0]);
        }

        UpdateSpriteSize();

        return *this;
    }

    AnimatedSprite& SetSize(const sf::Vector2f& size)
    {
        mBackground.setSize(size);

        UpdateSpriteSize();

        return *this;
    }

    AnimatedSprite& SetBackgroundColor(sf::Color color = sf::Color::Transparent)
    {
        mBackground.setFillColor(color);
        return *this;
    }

    AnimatedSprite& SetPosition(Alignment alignment, const sf::View& camera, const sf::Vector2f& offset = sf::Vector2f(0.f, 0.f)) override
    {
        sf::Vector2f position = offset;

        sf::Vector2f cameraCenter = camera.getCenter();
        sf::Vector2f cameraSize = camera.getSize();

        if (alignment == Alignment::CenterX || alignment == Alignment::Center)
        {
            position.x += camera.getCenter().x - mBackground.getSize().x / 2.0f;
        }

        if (alignment == Alignment::CenterY || alignment == Alignment::Center)
        {
            position.y += camera.getCenter().y - mBackground.getSize().y / 2.0f;
        }

        mSprite.setPosition(position);
        mBackground.setPosition(position);

        return *this;
    }

    AnimatedSprite& SetPosition(Alignment alignment, const View* parent, const sf::Vector2f& offset = sf::Vector2f(0.f, 0.f)) override
    {
        sf::Vector2f position = offset;

        sf::Vector2f cameraCenter = parent->GetCenter();
        sf::Vector2f cameraSize = parent->GetSize();

        if (alignment == Alignment::CenterX || alignment == Alignment::Center)
        {
            position.x += parent->GetCenter().x - mBackground.getSize().x / 2.0f;
        }

        if (alignment == Alignment::CenterY || alignment == Alignment::Center)
        {
            position.y += parent->GetCenter().y - mBackground.getSize().y / 2.0f;
        }

        mSprite.setPosition(position);
        mBackground.setPosition(position);

        return *this;
    }

    void Update(float deltaTime)
    {
        if (mTextures.empty()) return;

        mAnimationTime += deltaTime;
        if (mAnimationTime >= mFrameTime)
        {
            mAnimationTime = 0.f;
            mCurrentFrame = (mCurrentFrame + 1) % mTextures.size();
            mSprite.setTexture(mTextures[mCurrentFrame]);
        }
    }

    bool Contains(const sf::Vector2f& point) const override
    {
        return mSprite.getGlobalBounds().contains(point);
    }

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        target.draw(mBackground, states);
        target.draw(mSprite, states);
    }

private:
    void UpdateSpriteSize()
    {
        if (!mTextures.empty())
        {
            sf::FloatRect bounds = mBackground.getLocalBounds();
            mSprite.setScale(bounds.width / mSprite.getLocalBounds().width, bounds.height / mSprite.getLocalBounds().height);
        }
    }

private:
    sf::Sprite mSprite;
    sf::RectangleShape mBackground;
    std::vector<sf::Texture> mTextures;
    size_t mCurrentFrame;
    float mAnimationTime;
    float mFrameTime;
};