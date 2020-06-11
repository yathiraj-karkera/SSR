//
//  CBDOCharLabel.cpp
//  ChhotaBheemUpTheHill
//
//  Created by Deepthi on 5/11/17.
//
//

#include "DNDOCharLabel.hpp"
#include "DNDOManager.hpp"


DNDOCharLabel::DNDOCharLabel(TextHAlignment hAlignment /* = TextHAlignment::LEFT */,
                             TextVAlignment vAlignment /* = TextVAlignment::TOP */)
{
    DNDOManager::sharedManager()->_charArr.pushBack(this);
}

DNDOCharLabel::~DNDOCharLabel()
{
    
}

DNDOCharLabel* DNDOCharLabel::create()
{
    auto ret = new (std::nothrow) DNDOCharLabel;
    
    if (ret)
    {
        ret->autorelease();
    }
    
    return ret;
}

DNDOCharLabel* DNDOCharLabel::createWithSystemFont(const std::string& text, const std::string& font, float fontSize, const Size& dimensions /* = Size::ZERO */, TextHAlignment hAlignment /* = TextHAlignment::LEFT */, TextVAlignment vAlignment /* = TextVAlignment::TOP */)
{
    auto ret = new (std::nothrow) DNDOCharLabel(hAlignment,vAlignment);
    
    if (ret)
    {
        ret->setSystemFontName(font);
        ret->setSystemFontSize(fontSize);
        ret->setDimensions(dimensions.width, dimensions.height);
        ret->setString(text);
        
        ret->autorelease();
        
        return ret;
    }
    
    return nullptr;
}

DNDOCharLabel* DNDOCharLabel::createWithTTF(const std::string& text, const std::string& fontFile, float fontSize, const Size& dimensions /* = Size::ZERO */, TextHAlignment hAlignment /* = TextHAlignment::LEFT */, TextVAlignment vAlignment /* = TextVAlignment::TOP */)
{
    auto ret = new (std::nothrow) DNDOCharLabel(hAlignment,vAlignment);
    
    if (ret && ret->initWithTTF(text, fontFile, fontSize, dimensions, hAlignment, vAlignment))
    {
        ret->autorelease();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return nullptr;
}

DNDOCharLabel* DNDOCharLabel::createWithTTF(const TTFConfig& ttfConfig, const std::string& text, TextHAlignment hAlignment /* = TextHAlignment::CENTER */, int maxLineWidth /* = 0 */)
{
    auto ret = new (std::nothrow) DNDOCharLabel(hAlignment);
    
    if (ret && ret->initWithTTF(ttfConfig, text, hAlignment, maxLineWidth))
    {
        ret->autorelease();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return nullptr;
}
