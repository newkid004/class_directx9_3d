#include "example_19.h"

#include "managerList.h"
#include "particlePoint.h"
#include "particleQuad.h"
#include "particleCreater.h"

example_19::example_19(HINSTANCE hInstance, const SIZE & windowSize, int option) :
	direct3dApplication(hInstance, windowSize, option)
{
}

example_19::~example_19()
{
	SAFE_DELETE(_particleSystem);
}

void example_19::init(void)
{
	direct3dApplication::init();

	_particleSystem = createParticleSystem();
}

void example_19::update(void)
{
	direct3dApplication::update();
	_particleSystem->update();

	if (MN_KEY->keyDown(DIK_SPACE)) _particleSystem->particleEmitStart(1.0f);
}

void example_19::draw(void)
{
	direct3dApplication::draw();
	_particleSystem->draw();
}

particleQuad * example_19::createParticleSystem(void)
{
	particleEmitter::particle makeParam;
	particleQuad::mParams param;
	param.textureFilePath = "resource/texture/particle.png";
	param.emitterParam.maxNumParticle = 1000;
	param.emitterParam.numParticlePerSecond = 100;
	param.emitterParam.type = EParticleType::ORBIT;
	param.emitterParam.makeParam = particleCreater::makeParam(particleCreater::PC_ORBIT::MAKE, D3DXVECTOR2(0.2f, 0.2f), 10.0f, 3.0f);

	return new particleQuad(param);
}
