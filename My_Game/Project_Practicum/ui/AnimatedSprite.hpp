#pragma once

#include "View.h"
#include <vector>

class AnimatedSprite : public View
{
public:
    AnimatedSprite() : mCurrentFrame(0), mAnimationTime(0.f), mFrameTime(0.7f) {}

    AnimatedSprite& SetTextures(const std::vector<sf::Texture>& textures)
    {
        mTextures = textures;
        if (!mTextures.empty())
        {
            mSprite.setTexture(mTextures[0]);
        }
        return *this;
    }

    AnimatedSprite& SetScale(const sf::Vector2f& size)
    {
        mSprite.setScale(size);
        return *this;
    }

    AnimatedSprite& SetPosition(Alignment alignment, const sf::View& camera, const sf::Vector2f& offset = sf::Vector2f(0.f, 0.f))
    {
        sf::Vector2f position = offset;

        if (alignment == Alignment::CenterX || alignment == Alignment::Center)
        {
            position.x += camera.getCenter().x - mSprite.getLocalBounds().width / 2.0f;
        }

        if (alignment == Alignment::CenterY || alignment == Alignment::Center)
        {
            position.y += camera.getCenter().y - mSprite.getLocalBounds().height / 2.0f;
        }

        mSprite.setPosition(position);
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
        target.draw(mSprite, states);
    }

private:
    sf::Sprite mSprite;
    std::vector<sf::Texture> mTextures;
    size_t mCurrentFrame;
    float mAnimationTime;
    float mFrameTime;
};