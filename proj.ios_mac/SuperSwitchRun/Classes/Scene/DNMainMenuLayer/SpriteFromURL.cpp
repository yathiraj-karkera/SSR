//
//  SpriteFromUrl.cpp
//  SpriteFromUrl
//
//  Created by Guru on 11/27/14.
//
//

#include "SpriteFromUrl.h"
#include <fstream>


#pragma mark - shader black and white
//
// EffectSprite
//
static int tuple_sort( const std::tuple<ssize_t,Effect*,QuadCommand> &tuple1, const std::tuple<ssize_t,Effect*,QuadCommand> &tuple2 )
{
    return std::get<0>(tuple1) < std::get<0>(tuple2);
}

class Effect;

#pragma mark - Effect
bool Effect::initGLProgramState(const std::string &fragmentFilename)
{
    auto fileUtiles = FileUtils::getInstance();
    auto fragmentFullPath = fileUtiles->fullPathForFilename(fragmentFilename);
    auto fragSource = fileUtiles->getStringFromFile(fragmentFullPath);
    auto glprogram = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, fragSource.c_str());
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    _fragSource = fragSource;
#endif
    
    _glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
    _glprogramstate->retain();
    
    return _glprogramstate != nullptr;
}

Effect::Effect()
: _glprogramstate(nullptr)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    _backgroundListener = EventListenerCustom::create(EVENT_RENDERER_RECREATED,
                                                      [this](EventCustom*)
                                                      {
                                                          auto glProgram = _glprogramstate->getGLProgram();
                                                          glProgram->reset();
                                                          glProgram->initWithByteArrays(ccPositionTextureColor_noMVP_vert, _fragSource.c_str());
                                                          glProgram->link();
                                                          glProgram->updateUniforms();
                                                      }
                                                      );
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_backgroundListener, -1);
#endif
}

Effect::~Effect()
{
    CC_SAFE_RELEASE_NULL(_glprogramstate);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    Director::getInstance()->getEventDispatcher()->removeEventListener(_backgroundListener);
#endif
}

#pragma mark - Black n White
class EffectGreyScale : public Effect
{
public:
    CREATE_FUNC(EffectGreyScale);
    
protected:
    bool init() {
        //init with  Shader file
        initGLProgramState("blackWhiteShader.fsh");
        return true;
    }
};

#pragma mark - CREATE SPRITE FROM URL
void SpriteFromUrl::onEnter()
{
    Sprite::onEnter();
}
void SpriteFromUrl::onExit()
{
    
    Sprite::onExit();
}

//Set url
void SpriteFromUrl::setURL(const char * url)
{
    if(url==NULL)
        return;
    
    if(std::string("") == url){
        
        return;
    }
    //Call function to initiate image request from url
    this->requestForImage(url, this);
}

#pragma mark - Initialize image and property
bool SpriteFromUrl::init(const char * url,const char * samplepath,bool isBlackWhiteImageRequired)
{
    if(Sprite::initWithFile(samplepath))
    {
        //name will be placeholder imagename.
        //Set property of sprite url
        this->name=samplepath;
        this->isRoundProfilePicNeeded=false;
        this->isBlackandWhiteImageRequired=isBlackWhiteImageRequired;
        mUrl=url;
        setURL(mUrl);
        return true;
    }
    return false;
}

bool SpriteFromUrl::init(const char * url,const char * samplepath,bool isRoundProfilePicRequired,bool isBlackWhiteImageRequired)
{
    if(Sprite::initWithFile(samplepath))
    {
        //Set property of sprite url
        this->isRoundProfilePicNeeded=isRoundProfilePicRequired;
        this->isBlackandWhiteImageRequired=isBlackWhiteImageRequired;
        mUrl=url;
        this->name=samplepath;
        
        setURL(mUrl);
        return true;
    }
    return false;
}

#pragma mark - create Sprite
SpriteFromUrl * SpriteFromUrl::create(const char * url,const char * samplepath,bool isBlackWhiteImageRequired)
{
    SpriteFromUrl * sprite =new SpriteFromUrl;
    sprite->init(url,samplepath,isBlackWhiteImageRequired);
    sprite->autorelease();
    return sprite;
}
SpriteFromUrl * SpriteFromUrl::create(const char * url,const char * samplepath ,bool isRoundProfilePicRequired,bool isBlackWhiteImageRequired)
{
    SpriteFromUrl * sprite =new SpriteFromUrl;
    sprite->init(url,samplepath,isRoundProfilePicRequired,isBlackWhiteImageRequired);
    sprite->autorelease();
    return sprite;
}
SpriteFromUrl* SpriteFromUrl::createWithTexture(Texture2D *texture)
{
    SpriteFromUrl *sprite = new (std::nothrow) SpriteFromUrl();
    if (sprite && sprite->initWithTexture(texture))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

#pragma mark - Image request
void SpriteFromUrl::requestForImage(const char* url,SpriteFromUrl* sprite)
{
    //Check condition for image is already present in device
    if(this->setTextureFromFilePath(sprite, nameFromUrl(url))){
        
        return;
    }
    
    //Check condition url is Null
    if(url==NULL || sprite==NULL){
        return;
    }

    
    //Get name from url
    std::string strImage = nameFromUrl(url);
    
    //Create Http request
    HttpRequest* request = new HttpRequest();
    request->setUrl(url);
    request->setRequestType(HttpRequest::Type::GET);
      request->setResponseCallback(this, httpresponse_selector(SpriteFromUrl::onHttpRequestCompleted));
    
   // request->setResponseCallback(CC_CALLBACK_2(SpriteFromUrl::onHttpRequestCompleted, this));
    request->setTag(strImage.c_str());
    
    HttpClient::getInstance()->send(request);
    request->release();

}
//Create image from device file path
bool SpriteFromUrl::setTextureFromFilePath(SpriteFromUrl * sprite,std::string path)
{
    //Get device writable path
    std::string cachepath = FileUtils::getInstance()->getWritablePath();
    cachepath+=path;
    
    //Check if image path is already present in device path
    Texture2D * texture =Director::getInstance()->getTextureCache()->addImage(cachepath.c_str());
    if (texture)
    {
        //condition check to round pic
        if(!this->isRoundProfilePicNeeded)
        {
            //Function call to change the dimention of Sprite according to url image
            this->changeDimentionToUrlSprite(texture);
            this->setTexture(texture);
            
                    }
        else
        {
            //Create round pic
            Sprite *newSpr = Sprite::createWithTexture(texture);
            this->createCircleImage(newSpr);
        }
        
        return true;
    }
    return false;
}

#pragma mark - Loading animation
void SpriteFromUrl::addLoadingAnimation()
{
    //Name of loading plist file
    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("LoadingAnim.plist", "LoadingAnim.png");

    this->loadingSp=Sprite::createWithSpriteFrameName("loader_0.png");
    this->addChild(this->loadingSp,10);
    this->loadingSp->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    
    Vector<SpriteFrame*> animFrames(120);
    
    char str[160] = {0};
    for(int i = 0; i < 46; i++)
    {
        sprintf(str, "loader_%d.png",i);
        auto frame = cache->getSpriteFrameByName(str);
        animFrames.pushBack(frame);
    }
    
    Animation* loadAnim = Animation::createWithSpriteFrames(animFrames, 0.02f);
    RepeatForever *rep=RepeatForever::create(Animate::create(loadAnim));
    rep->setTag(2);
    this->loadingSp->runAction(rep);
}

#pragma mark - On Http responce
void SpriteFromUrl::onHttpRequestCompleted(HttpClient* pSender, HttpResponse* pResponse)
{
    Size winSize=Director::getInstance()->getWinSize();
    HttpResponse* response = pResponse;
    
    
    //Response data
    if (!response)
    {
        log("No Response");
        return;
    }
    
    if (!response->isSucceed())
    {
        log("response failed");
        log("error buffer: %s", response->getErrorBuffer());
        //MessageBox("error in downloaded", "Image");
        return;
    }
    
    std::vector< char> *buffer = response->getResponseData();
    const unsigned char* dataVal = (const unsigned char*)(&(buffer->front()));
    
    //Create image with data
    Image * img=new Image();
    img->initWithImageData(dataVal, buffer->size());
    
    // Save image file to device.
    std::string writablePath = FileUtils::getInstance()->getWritablePath();
    writablePath.append(response->getHttpRequest()->getTag());
    //img->saveToFile(writablePath.c_str());
    
    std::ofstream outfile(writablePath.c_str());
    for (unsigned int i = 0; i < buffer->size(); i++)
        outfile<<(*buffer)[i];
    outfile.close();
    
    Sprite* pSprite;
    Texture2D * texture =Director::getInstance()->getTextureCache()->addImage(writablePath.c_str());
    if (texture)
    {
        pSprite = Sprite::createWithTexture(texture);
    }
    
    //Create Sprite from downloaded image
    //Sprite* pSprite = Sprite::create(writablePath.c_str());
    
    //Condition check if circle image is required
    if(!this->isRoundProfilePicNeeded)
    {
        //Function call to change the dimention of Sprite according to url image
        this->changeDimentionToUrlSprite(pSprite->getTexture());

        //Set url image
        this->setTexture(pSprite->getTexture());
        
    }
    else
    {
        //Create circle image
        if(texture!=NULL)
        {
             this->createCircleImage(pSprite);
        }
       
    }
}
//Resize sample image to url image size
void SpriteFromUrl::changeDimentionToUrlSprite(Texture2D* urlSpr)
{
    //Adjust the current texture size to new size
    auto currentSprContentSize = this->getContentSize();
    auto updatedSprFromUrlContentSize =urlSpr->getContentSize();
    
    //Calculate scale factor if SAample image is not same as url image
    auto scaleX = updatedSprFromUrlContentSize.width/currentSprContentSize.width;
    auto scaleY = updatedSprFromUrlContentSize.height/currentSprContentSize.height;
    
    //Set scale to set image size for new updated image from url
    this->setScale(scaleX,scaleY);

}

#pragma mark- Round circle
void SpriteFromUrl::createCircleImage(Sprite* newSpr)
{
    //
    auto* maskCircleImage =Sprite::create(this->name);
    
    //Adjust the url image to circle image
    auto currentSprContentSize = maskCircleImage->getContentSize();
    auto updatedSprFromUrlContentSize =newSpr->getContentSize();
    
    auto scaleX = currentSprContentSize.width/updatedSprFromUrlContentSize.width;
    auto scaleY = currentSprContentSize.height/updatedSprFromUrlContentSize.height;
    
    //Resize url image size to placeholder image size
    newSpr->setScale(scaleX,scaleY);
    
    float puzzlePartXPos;
    float puzzlePartYPos;
    
    //Set position for Puzzle part box sprite
    puzzlePartXPos=maskCircleImage->getContentSize().width/2;
    puzzlePartYPos=maskCircleImage->getContentSize().height/2;
    
    //Set pos to sample image center pos
    maskCircleImage->setPosition(Vec2(puzzlePartXPos, puzzlePartYPos));
    
    //create circle sprite
    SpriteFromUrl *proflepic= this->maskedSpriteWithSprite(newSpr, maskCircleImage,Vec2(0, 0));
    proflepic->setPosition(Vec2(this->getContentSize().width/2,this->getContentSize().height/2));
    this->addChild(proflepic);
    
   }
//Mask image to placeholder image
SpriteFromUrl* SpriteFromUrl::maskedSpriteWithSprite(Sprite* pTextureSprite, Sprite* pMaskSprite,Vec2 profilePicPos)
{
    // convert the texture sprite position into mask sprite coordinate system
    pTextureSprite->setPosition(Vec2((pTextureSprite->getContentSize().width*pTextureSprite->getScaleX())/2 + pMaskSprite->getContentSize().width/2-pMaskSprite->getPositionX(), (pTextureSprite->getContentSize().height*pTextureSprite->getScaleY())/2 + pMaskSprite->getContentSize().height/2-pMaskSprite->getPositionY()));
    
    //Create render texture with width and height of round
    RenderTexture *rt = RenderTexture::create(pMaskSprite->getContentSize().width, pMaskSprite->getContentSize().height);
    
    //Use blend function to cut image according sample image
    BlendFunc bfMask = BlendFunc();
    bfMask.src = GL_ONE;
    bfMask.dst = GL_ZERO;
    pMaskSprite->setBlendFunc(bfMask);
    
    // turn off anti-aliasing around the mask sprite
    pMaskSprite->getTexture()->setAntiAliasTexParameters();
    
    BlendFunc bfTexture = BlendFunc();
    bfTexture.src = GL_DST_ALPHA;
    bfTexture.dst = GL_ZERO;
    pTextureSprite->setBlendFunc(bfTexture);
    
    pTextureSprite->getTexture()->setAntiAliasTexParameters();
    
    //Begin render
    rt->begin();
    pMaskSprite->visit();
    pTextureSprite->visit();
    rt->end();
    
    //Generate the resulting sprite
    SpriteFromUrl *pOutcome = SpriteFromUrl::createWithTexture(rt->getSprite()->getTexture());
    pOutcome->setFlippedY(true);
    pOutcome->getTexture()->setAntiAliasTexParameters();
    
    return pOutcome;
}

#pragma mark - get name from url utility
std::string SpriteFromUrl::nameFromUrl(std::string url)
{
    std::string name = url;
    name.erase( remove( name.begin(), name.end(), '/' ), name.end() );
    name.erase( remove( name.begin(), name.end(), ':' ), name.end() );
    return name;
}
