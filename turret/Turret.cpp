#include "Turret.h"
using namespace turret;
//constructor of class Turret
Turret::Turret(int lv){
	power = powerList[lv - 1];
	price = priceList[lv - 1];
	range = rangeList[lv - 1];
	discount = discountList[lv - 1];
	level = lv;
}
//definition of sellTurret()
float sellTurret(Turret& t){
	float re = t.getPrice() * t.getDiscount();
	t.~Turret();
	return re;
}
//definition of upgrade()
float upgrade(Turret& t){
	int currentLv = t.getLv();
	t.setLv(currentLv + 1);
	float re = priceList[currentLv] - priceList[currentLv - 1];
	t.setPower(currentLv);
	t.setRange(currentLv);
	t.setPrice(currentLv);
	t.setDiscount(currentLv);
	return re;
}
