#ifndef INCLUDED_SOUND_PLAYER_H
#define INCLUDED_SOUND_PLAYER_H

#include <SFML\Audio.hpp>
#include <string>
#include <vector>
#include <memory>

class SoundPlayer
{
public:

	typedef std::unique_ptr<sf::Sound> SoundPtr;

	void playSound(const std::string &soundName, const sf::Vector2f &position, float volume = 100);
	void playMusic(const std::string &musicName, bool looping = true, float volume = 100);
	void pauseMusic(const std::string &musicName);
	void stopMusic(const std::string &musicName);
	
	// Set master volume of system
	void setVolume(float volume); 

	void update(float deltaTime, const sf::Vector2f &playerPosition);

	static SoundPlayer& instance();

private:

	SoundPlayer();

	// Delay between audio flushes (removing non-playing sounds)
	float m_currentDelay;
	const int m_delayMax;

	std::vector<SoundPtr> m_sounds;
};

#endif