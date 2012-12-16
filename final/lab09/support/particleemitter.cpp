#include "particleemitter.h"

ParticleEmitter::ParticleEmitter(float3 color, float3 velocity,
                                 float scale, float decay, float speed,
                                 unsigned maxParticles) :
                    m_maxParticles(maxParticles), m_speed(speed),
                    m_fuzziness(decay), m_scale(scale), m_color(color), m_velocity(velocity)
{
    m_particles = new Particle[maxParticles];
    resetParticles();

    for (unsigned i = 0; i < m_maxParticles; ++i)
        m_particles[i].active = false;
}

ParticleEmitter::~ParticleEmitter()
{
    if (m_particles)
    {
        delete[] m_particles;
        m_particles = 0;
    }

    glDeleteTextures(1,&m_textureID);
}

/**
  * You need to fill this in.
  *
  * Resets the particle at the given index to its initial state. Should reset the
  * position, direction, force, color, life, and decay of the particle.
  */
void ParticleEmitter::resetParticle(unsigned i)
{
    m_particles[i].pos.zero();

    m_particles[i].life = 2.0;

    m_particles[i].decay = m_fuzziness;//urand(m_littleR,m_bigR);

    m_particles[i].color = m_color;

    //m_particles[i].force.x = urand((0 - m_fuzziness)*0.01f,m_fuzziness*0.01f) + m_force.x;
    //m_particles[i].force.y = urand((0 - m_fuzziness)*0.01f,m_fuzziness*0.01f) + m_force.y;
    //m_particles[i].force.z = urand((0 - m_fuzziness)*0.01f,m_fuzziness*0.01f) + m_force.z;

    m_particles[i].dir.x = urand(0,m_velocity.x);//urand((0 - m_velocity.x),m_velocity.x);
    m_particles[i].dir.y = urand(0,m_velocity.y);//urand((0 - m_velocity.y),m_velocity.y);
    m_particles[i].dir.z = urand(0,m_velocity.z);//urand((0 - m_velocity.z),m_velocity.z);
}

void ParticleEmitter::setRadius(float little, float big) {
    m_littleR = little;
    m_bigR = big;
}

/**
  * Resets all particles in this emitter to their initial states
  */
void ParticleEmitter::resetParticles()
{
    for (unsigned i = 0; i < m_maxParticles; i++)
        resetParticle(i);
}

/**
  * You need to fill this in.
  *
  * Performs one step of the particle simulation. Should perform all physics
  * calculations and maintain the life property of each particle.
  */
void ParticleEmitter::updateParticles()
{
    for(unsigned i = 0; i < m_maxParticles; ++i)
    {
        if (!m_particles[i].active) {
            m_particles[i].active = true;
            resetParticle(i);

            continue;
        }

        m_particles[i].pos += m_particles[i].dir*m_speed;

        //m_particles[i].life -= m_particles[i].decay;

        //if (m_particles[i].life < 0)
        if (m_particles[i].pos.getDistance(float3::Zero()) > m_particles->decay)
            m_particles[i].active = false;
    }
}

/**
  * You need to fill this in.
  *
  * Draws each particle as a small, texture-mapped square of side-length m_scale.
  * Each square should be in the X/Y plane at Z = the particle's position's Z-coordinate.
  */
void ParticleEmitter::drawParticles(Matrix4x4 trans)
{
    //glBindTexture(GL_TEXTURE_2D, m_textureID);

    //glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glEnable(GL_BLEND);

    //glDepthMask(GL_FALSE);

    glBegin(GL_QUADS);

    float scale = m_scale/2.0;

    for (int i=0;i<m_maxParticles;i++) {

        glColor4f(m_particles[i].color.r,m_particles[i].color.g,m_particles[i].color.b,m_particles[i].life);

        Vector4 pos = Vector4::zero();
        pos.x = m_particles[i].pos.x;
        pos.y = m_particles[i].pos.y;
        pos.z = m_particles[i].pos.z;
        pos.w = 1;
        pos = trans * pos;

        glVertex3f(pos.x-scale,pos.y-scale,pos.z);

        glVertex3f(pos.x-scale,pos.y+scale,pos.z);

        glVertex3f(pos.x+scale,pos.y+scale,pos.z);

        glVertex3f(pos.x+scale,pos.y-scale,pos.z);


        glVertex3f(pos.x+scale,pos.y-scale,pos.z);

        glVertex3f(pos.x+scale,pos.y+scale,pos.z);

        glVertex3f(pos.x-scale,pos.y+scale,pos.z);

        glVertex3f(pos.x-scale,pos.y-scale,pos.z);


    }

    //glDepthMask(GL_TRUE);

    glEnd();
}
