//
//  FQGameUtility.hpp
//  Game       : FreakQuiz
//  Created by : Dhanashree Kamath K on 08/17//16.
//  Game Studio:JUEGOSTUDIOS
//

#ifndef DNGameUtility_hpp
#define DNGameUtility_hpp

#include <stdio.h>
#include "cocos2d.h"

#include "extensions/cocos-ext.h"
#include "json/rapidjson.h"
//#include "json/filestream.h"
#include "json/document.h"
//#include "PGConfig.h"

#include "network/HttpRequest.h"
#include "network/HttpResponse.h"
#include "network/HttpClient.h"

using namespace cocos2d;

namespace std
{
    
    namespace   {    inline string char2hex( char dec )
        {
            char dig1 = (dec&0xF0)>>4;
            char dig2 = (dec&0x0F);
            if ( 0<= dig1 && dig1<= 9) dig1+=48;    //0,48inascii
            if (10<= dig1 && dig1<=15) dig1+=97-10; //a,97inascii
            if ( 0<= dig2 && dig2<= 9) dig2+=48;
            if (10<= dig2 && dig2<=15) dig2+=97-10;
            
            string r;
            r.append( &dig1, 1);
            r.append( &dig2, 1);
            return r;
        }
        
    }
    
    
    
    inline string urlencode(const string &c)
    {
        
        string escaped="";
        int max = c.length();
        for(int i=0; i<max; i++)
        {
            if ( (48 <= c[i] && c[i] <= 57) ||//0-9
                (65 <= c[i] && c[i] <= 90) ||//abc...xyz
                (97 <= c[i] && c[i] <= 122) || //ABC...XYZ
                (c[i]=='~' || c[i]=='!' || c[i]=='*' || c[i]=='(' || c[i]==')' || c[i]=='\'')
                )
            {
                escaped.append( &c[i], 1);
            }
            else
            {
                escaped.append("%");
                escaped.append( char2hex(c[i]) );//converts char 255 to string "ff"
            }
        }
        return escaped;
    }
    
    inline string urlDecode(string &SRC) {
        string ret;
        char ch;
        int i, ii;
        for (i=0; i<SRC.length(); i++) {
            if (int(SRC[i])==37) {
                sscanf(SRC.substr(i+1,2).c_str(), "%x", &ii);
                ch=static_cast<char>(ii);
                ret+=ch;
                i=i+2;
            }else if(SRC[i]=='+')
            { ret+=' ';
            }
            else {
                ret+=SRC[i];
            }
        }
        return (ret);
    }
    
    
    inline std::string strtruncate(std::string trunk, size_t size)
    {
        if(trunk.size()>size)
        {
            trunk= trunk.substr(0,size);
            trunk+="..";
        }
        
        return trunk;
        
    }
    
}


class DNGameUtility: Ref
{
    
private:
    
    DNGameUtility(void);
    ~DNGameUtility(void);
    
public:
    
    static void getResponseBuffer(void *data , rapidjson::Document &  document);
    static bool checkIsNetworkAvailable();
    //    static std::string GetFilePath(std::string folderNam,std::string fileName);
    //    static std::string GetStringFromThePath(std::string pathName);
};
#endif /* DNGameUtility_hpp */
