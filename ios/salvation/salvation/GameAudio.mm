//
//  GameAudio.m
//  puzzlecube
//
//  Created by Alex on 5/2/13.
//
//

#import "GameAudio.h"
//#import "SimpleAudioEngine.h"

#include <string>
#include "MyGUI.h"

@implementation GameAudio

void playBackgroundMusic(std::string filename)
{

}

void stopBackgroundMusic()
{

}

unsigned int playEffect(std::string filename)
{
    
}

unsigned int playEffect(std::string filename, float pitch, float pan, float gain)
{
    
}

void playButtonOver(MyGUI::Widget* _sender, MyGUI::Widget* _old)
{
    
}

void playButtonClick(MyGUI::WidgetPtr _sender)
{
    
}

void playButtonClickAlt(MyGUI::WidgetPtr _sender)
{
    
}

/*

void playBackgroundMusic(std::string filename)
{
    // hack for now
    [SimpleAudioEngine sharedEngine].backgroundMusicVolume = 0.25;
    [[SimpleAudioEngine sharedEngine] playBackgroundMusic:[NSString stringWithCString:filename.c_str() encoding:[NSString defaultCStringEncoding]]];
}

void stopBackgroundMusic()
{
    [[SimpleAudioEngine sharedEngine] stopBackgroundMusic];
}

unsigned int playEffect(std::string filename)
{
    return [[SimpleAudioEngine sharedEngine] playEffect:[NSString stringWithCString:filename.c_str() encoding:[NSString defaultCStringEncoding]]];
}

unsigned int playEffect(std::string filename,float pitch, float pan, float gain)
{
    return [[SimpleAudioEngine sharedEngine] playEffect:[NSString stringWithCString:filename.c_str() encoding:[NSString defaultCStringEncoding]] pitch:pitch pan:pan gain:gain];
}
*/

@end


