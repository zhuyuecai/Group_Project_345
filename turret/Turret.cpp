#include "Turret.h"
//using namespace turret;
//constructor of class Turret

turret::Turret::Turret(int lv){
	power = powerList[lv - 1];
	price = priceList[lv - 1];
	range = rangeList[lv - 1];
	discount = discountList[lv - 1];
	level = lv;
}
//definition of sellTurret()
float turret::sellTurret(turret::Turret& t){
	float re = t.getPrice() * t.getDiscount();
	t.~Turret();
	return re;
}
//definition of upgrade()
float turret::upgrade(turret::Turret& t){
	int newLv = t.getLv()+1;
	t.setLv(newLv);
	float re = priceList[newLv] - priceList[newLv - 1];
	t.setPower(newLv);
	t.setRange(newLv);
	t.setPrice(newLv);
	t.setDiscount(newLv);
	return re;
}
