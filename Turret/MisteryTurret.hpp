#ifndef MISTERYTURRET_HPP
#define MISTERYTURRET_HPP
#include "Turret.hpp"

class MisteryTurret: public Turret {
public:
	static const int Price;
    MisteryTurret(float x, float y);
	void CreateBullet() override;
};
#endif // MACHINEGUNTURRET_HPP
