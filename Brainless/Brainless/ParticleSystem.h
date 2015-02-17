#ifndef INCLUDED_PARTICLE_SYSTEM_H
#define INCLUDED_PARTICLE_SYSTEM_H

#include <SFML\Graphics.hpp>

class ParticleSystem : public sf::Drawable, public sf::Transformable
{
public:


	void addParticles(std::size_t count, const sf::Vector2f &position, const sf::Color &color);
	void setEmitter(sf::Vector2f position);

	void update(float deltaTime);

	static ParticleSystem& instance();

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:

	ParticleSystem();

	struct Particle
	{
		sf::Vector2f velocity;
		sf::Time lifetime;
	};

	void addParticle(const sf::Color &color);
	void resetParticle(std::size_t index);

	std::vector<Particle> m_particles;
	sf::VertexArray m_vertices;
	sf::Time m_lifetime;
	sf::Vector2f m_emitter;
};

#endif