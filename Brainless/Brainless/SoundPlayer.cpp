#include "SoundPlayer.h"
#include "ResourceLoader.h"

SoundPlayer::SoundPlayer()
	:
	m_currentDelay(0),
	m_delayMax(3)
{
	setVolume(100.f);
}

void SoundPlayer::playSound(const std::string &soundName, const sf::Vector2f &position, float volume)
{
	SoundPtr sound(new sf::Sound());
	sound->setBuffer(ResourceLoader::instance().retrieveSound(soundName));
	sound->setRelativeToListener(false);
	sound->setPosition(position.x, position.y, 0);
	sound->setVolume(volume);
	sound->setAttenuation(2);
	sound->setMinDistance(100);

	m_sounds.push_back(std::move(sound));
	m_sounds.back()->play();
}
void SoundPlayer::playMusic(const std::string &musicName, bool looping, float volume)
{
	sf::Music &music = ResourceLoader::instance().retrieveMusic(musicName);
	music.setRelativeToListener(false);
	music.setVolume(volume);
	music.setLoop(looping);
	music.play();
}
void SoundPlayer::pauseMusic(const std::string &musicName)
{
	ResourceLoader::instance().retrieveMusic(musicName).pause();
}
void SoundPlayer::stopMusic(const std::string &musicName)
{
	ResourceLoader::instance().retrieveMusic(musicName).stop();
}

void SoundPlayer::killAllSounds()
{
	for (auto &sound : m_sounds)
		sound->stop();

	m_sounds.clear();
}

void SoundPlayer::setVolume(float volume)
{
	sf::Listener::setGlobalVolume(volume);
}
float SoundPlayer::getVolume() const
{
	return sf::Listener::getGlobalVolume();
}

void SoundPlayer::update(float deltaTime, const sf::Vector2f &playerPosition)
{
	sf::Listener::setPosition(playerPosition.x, playerPosition.y, 0);

	m_currentDelay += deltaTime;

	if (m_currentDelay >= m_delayMax)
	{
		m_currentDelay = m_currentDelay - m_delayMax;

		// Find sounds that aren't playing, and mark them for deletion
		std::vector<std::size_t> soundsToRemove;
		for (std::size_t i = 0; i < m_sounds.size(); i++)
		{
			if (m_sounds[i]->getStatus() == sf::Sound::Stopped)
				soundsToRemove.push_back(i);
		}

		// Remove unused sounds in separate loop to avoid errors when doing it in the same loop
		for (int i = soundsToRemove.size()-1; i > 0; i--)
			m_sounds.erase(m_sounds.begin() + soundsToRemove[i]);
	}
}

SoundPlayer& SoundPlayer::instance()
{
	static SoundPlayer player;
	return player;
}
