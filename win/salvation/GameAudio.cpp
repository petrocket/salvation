#include "stdafx.h"

sf::Music gMusic;

typedef struct SoundEffect 
{
	sf::SoundBuffer buffer;
	sf::Sound sound;
} SoundEffect;

typedef std::map<std::string,SoundEffect *> SoundEffectsMap;

SoundEffectsMap gSoundEffects;

std::string relativePathForFile(std::string filename)
{
	Ogre::ResourceGroupManager *mgr = Ogre::ResourceGroupManager::getSingletonPtr();
	std::string foundPath = filename;
	Ogre::FileInfoListPtr fi = mgr->findResourceFileInfo("General", foundPath);
	Ogre::FileInfoList::iterator it = fi->begin();

	if(it != fi->end()) {
		foundPath = it->archive->getName() + "/" + foundPath;
	}

	return foundPath;
}

void playBackgroundMusic(std::string filename)
{
	if (gMusic.openFromFile( relativePathForFile(filename) )) {
		gMusic.play();
		gMusic.setVolume(50);
		gMusic.setLoop(true);
	}
}

void stopBackgroundMusic()
{
	gMusic.stop();
}

unsigned int playEffect(std::string filename, float pitch, float pan, float gain)
{

	std::string f = relativePathForFile(filename);
	SoundEffectsMap::iterator ii = gSoundEffects.find(f);

	SoundEffect *effect;
	if(ii == gSoundEffects.end()) {
		effect = new SoundEffect();
		if(effect->buffer.loadFromFile(f)) {
			effect->sound.setBuffer(effect->buffer);
		}
		gSoundEffects[f] = effect;
	}
	else {
		effect = ii->second;
	}

	effect->sound.setVolume(gain * 100.0);
	effect->sound.setPitch(pitch);
	effect->sound.setPosition(pan,0,0);
	effect->sound.play();

	return 0;
}

unsigned int playEffect(std::string filename)
{
	return playEffect(filename,1.0,0.0,1.0);
}

void playButtonOver(MyGUI::Widget* _sender, MyGUI::Widget* _oldr)
{
	playEffect("hover.ogg");
}

void playButtonClick(MyGUI::WidgetPtr _sender)
{
	playEffect("click.ogg");
}

void playButtonClickAlt(MyGUI::WidgetPtr _sender)
{
	playEffect("clickalt.ogg");
}
