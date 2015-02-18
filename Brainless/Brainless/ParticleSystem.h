#ifndef INCLUDED_PARTICLE_SYSTEM_H
#define INCLUDED_PARTICLE_SYSTEM_H

#include <SFML\Graphics.hpp>

class ParticleSystem
{
public:


	void addParticles(std::size_t count, const sf::Vector2f &position, const sf::Color &color,
		const sf::Vector2f &lifetimeRange = sf::Vector2f(1, 1),
		const sf::Vector2f &angleRange = sf::Vector2f(0, 360),
		const sf::Vector2f &rotationSpeedRange = sf::Vector2f(0, 0),
		const sf::Vector2f &speedRangeX = sf::Vector2f(-100, 100),
		const sf::Vector2f &speedRangeY = sf::Vector2f(-100, 100),
		const sf::Vector2f &gravity = sf::Vector2f(0, 0));

	void update(float deltaTime);
	void draw();

	static ParticleSystem& instance();

private:


	ParticleSystem();

	sf::Texture m_particleTexture;

	struct Particle
	{
		sf::Sprite sprite;
		sf::Vector2f velocity;
		sf::Vector2f gravity;
		float rotationSpeed;
		float lifeTime, maxLifeTime;
	};

	std::vector<Particle> m_particles;
};

#endif