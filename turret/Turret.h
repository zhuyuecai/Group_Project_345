/*Header file for the class turret, It defines the constructor and public interface for the class Turret*/
#ifndef TURRET_H
#define TURRET_H
namespace turret{
	//constants
	const int num_lv = 10;
	//constant parameters should be ajusted later according to the main method algorithm
	const float powerList[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	const float rangeList[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	const float discountList[] = {0.05f, 0.1f, 0.15f, 0.2f, 0.25f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f};
	const float priceList [] ={1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

	class Turret{
	private:
		//variables
		float power;
		float range;
		float discount;
		float price;
		int level;
	public:
		//constructor
		Turret(int lv);
		//getters
		float getPower(){ return power; }
		float getRange(){ return range; }
		float getPrice(){ return price; }
		float getDiscount(){ return discount; }
		int getLv(){ return level; }
		//mutaters 
		void setPower(int lv){ power = powerList[lv - 1]; }
		void setPrice(int lv){ price = priceList[lv - 1]; }
		void setRange(int lv){ range = rangeList[lv - 1]; }
		void setDiscount(int lv){ discount = discountList[lv - 1]; }
		void setLv(int lv){ level = lv; }

	};

	//function of selling the turret, the value return is the money you gain after selling it
	float sellTurret(Turret& t);
	//upgrade() with increment the level of the input turret by 1 and hence evey parameters of it accordingly. 
	//the return value is the money you pay for the upgrade.
	float upgrade(Turret& t);
}

#endif