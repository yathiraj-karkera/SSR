//
//  CBDOCharLabel.hpp
//  ChhotaBheemUpTheHill
//
//  Created by Deepthi on 5/11/17.
//
//

#ifndef DNDOCharLabel_hpp
#define DNDOCharLabel_hpp

#include <stdio.h>
#include "cocos2d.h"

using namespace cocos2d;

class DNDOCharLabel : public Label
{
public:
    
    DNDOCharLabel(TextHAlignment hAlignment = TextHAlignment::LEFT,
                  TextVAlignment vAlignment = TextVAlignment::TOP);
    ~DNDOCharLabel();
    
    static DNDOCharLabel* create();
    static DNDOCharLabel* createWithSystemFont(const std::string& text, const std::string& font, float fontSize,
                                               const Size& dimensions = Size::ZERO, TextHAlignment hAlignment = TextHAlignment::LEFT,
                                               TextVAlignment vAlignment = TextVAlignment::TOP);
    static DNDOCharLabel * createWithTTF(const std::string& text, const std::string& fontFilePath, float fontSize,
                                         const Size& dimensions = Size::ZERO, TextHAlignment hAlignment = TextHAlignment::LEFT,
                                         TextVAlignment vAlignment = TextVAlignment::TOP);
    static DNDOCharLabel* createWithTTF(const TTFConfig& ttfConfig, const std::string& text,
                                        TextHAlignment hAlignment = TextHAlignment::LEFT, int maxLineWidth = 0);
    

    
};


#endif /* DNDOCharLabel_hpp */
