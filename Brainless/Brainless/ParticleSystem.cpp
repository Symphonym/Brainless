#include "ParticleSystem.h"
#include "Utility.h"
#include "Renderer.h"

ParticleSystem::ParticleSystem()
{
	sf::Image parImg;
	parImg.create(5, 5, sf::Color::White);

	m_particleTexture.loadFromImage(parImg);

}

ParticleSystem& ParticleSystem::instance()
{
	static ParticleSystem inst;
	return inst;
}

void ParticleSystem::addParticles(std::size_t count, const sf::Vector2f &position, const sf::Color &color,
	const sf::Vector2f &lifetimeRange,
	const sf::Vector2f &angleRange,
	const sf::Vector2f &rotationSpeedRange,
	const sf::Vector2f &speedRangeX,
	const sf::Vector2f &speedRangeY,
	const sf::Vector2f &gravity)
{

	for (std::size_t i = 0; i < count; i++)
	{
		Particle particle;
		particle.sprite.setTexture(m_particleTexture);
		particle.sprite.setPosition(position);
		particle.sprite.setColor(color);
		particle.sprite.setRotation(Utility::randomValueBetween(angleRange.x, angleRange.y));
		particle.velocity = sf::Vector2f(
			Utility::randomValueBetween(speedRangeX.x, speedRangeX.y),
			Utility::randomValueBetween(speedRangeY.x, speedRangeY.y));
		particle.lifeTime = 0;
		particle.maxLifeTime = Utility::randomValueBetween(lifetimeRange.x, lifetimeRange.y);
		particle.rotationSpeed = Utility::randomValueBetween(rotationSpeedRange.x, rotationSpeedRange.y);
		particle.gravity = gravity;

		m_particles.push_back(particle);
	}
}

void ParticleSystem::update(float deltaTime)
{
	for (auto itr = m_particles.begin(); itr != m_particles.end();)
	{

		// Update lifetime
		Particle& particle = *itr;
		particle.lifeTime += deltaTime;

		// If the particle is dead, remove it
		if (particle.lifeTime >= particle.maxLifeTime)
		{
			itr = m_particles.erase(itr);
		}
		else
		{
			float ratio = particle.lifeTime / particle.maxLifeTime;

			particle.velocity += particle.gravity;
			particle.sprite.move(particle.velocity * deltaTime);
			particle.sprite.rotate(particle.rotationSpeed * deltaTime);

			sf::Color currentColor = particle.sprite.getColor();
			currentColor.a = 255 - static_cast<sf::Uint8>(ratio * 255);
			particle.sprite.setColor(currentColor);

			itr++;
		}

	}
}

void ParticleSystem::draw()
{
	for (auto &particle : m_particles)
		Renderer::instance().drawAbove(particle.sprite);
}
