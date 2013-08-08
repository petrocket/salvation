#ifndef __GameAudioInterface_h
#define __GameAudioInterface_h

void playBackgroundMusic(std::string filename);
void stopBackgroundMusic();

unsigned int playEffect(std::string filename);
unsigned int playEffect(std::string filename, float pitch, float pan, float gain);

void playButtonOver(MyGUI::Widget* _sender, MyGUI::Widget* _old);
void playButtonClick(MyGUI::WidgetPtr _sender);

void playButtonClickAlt(MyGUI::WidgetPtr _sender);

#endif