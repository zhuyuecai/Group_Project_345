/*Header file for the class turret, It defines the constructor and public interface for the class Turret, 
requires an existed class account*/


#ifndef TURRET_H
#define TURRET_H
namespace turret{
public:
	const int num_lv = 10;
	const float* powerList = new float[num_lv] {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	const float* rangeList = new float[num_lv] {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};;
	const float* discountList = new float[num_lv] {0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.4, 0.5, 0.6, 0.7};;
	const float* priceList = new float[num_lv] {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	class Turret{
	public:
		Turret(int lv);
		float getPower();
		float getRange();
		float getPrice();
		float getDiscount();
		int getLv();
	private :
		float power;
		float range;
		float discount;
		float price;
		int level;
		void setPower(int lv);
		void setPrice(int lv);
		void setRange(int lv);
		void setDiscount(int lv);
		void setLevel(int lv);
	};
	public void sell_Turret(Turret t);
	inline void Turret::setPower(int lv){power = powerList[lv - 1];}
	inline void Turret::setRange(int lv){range = rangeList[lv - 1];}
	inline void Turret::setPrice(int lv){price = priceList[lv - 1];}
	inline void Turret::setDiscount(int lv){discount = discountList[lv - 1];}
	inline void Turret::setLevel(int lv){level = lv ;}
	inline float getPower(){return power;}
	inline float getPrice(){ return price; }
	inline float getRange(){ return range; }
	inline float getDiscount(){ return discount; }
	inline int getLevel(){ return level; }
}

#endif