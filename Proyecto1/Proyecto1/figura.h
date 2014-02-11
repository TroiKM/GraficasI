#include <iostream>

class Figura{
private:
	int zonaSelec;
	float angManoDer;
	float angManoIzq;
	float angCodoDer;
	float angCodoIzq;
	float angHombroDer;
	float angHombroIzq;
	float angCaderaDer;
	float angCaderaIzq;
	float angRodillaDer;
	float angRodillaIzq;
	float angTobilloDer;
	float angTobilloIzq;
	float angCabeza;

public:
	Figura();
	void setZonaSelec(int zona);
	int getZonaSelec();
	void setAngManoDer(float ang);
	int getAngManoDer();
	void setAngManoIzq(float ang);
	int getAngManoIzq();
	void setAngCodoDer(float ang);
	int getAngCodoDer();
	void setAngCodoIzq(float ang);
	int getAngCodoIzq();
	void setAngHombroDer(float ang);
	int getAngHombroDer();
	void setAngHombroIzq(float ang);
	int getAngHombroIzq();
	void setAngCaderaDer(float ang);
	int getAngCaderaDer();
	void setAngCaderaIzq(float ang);
	int getAngCaderaIzq();
	void setAngRodillaDer(float ang);
	int getAngRodillaDer();
	void setAngRodillaIzq(float ang);
	int getAngRodillaIzq();
	void setAngTobilloDer(float ang);
	int getAngTobilloDer();
	void setAngTobilloIzq(float ang);
	int getAngTobilloIzq();
	void setAngCabeza(float ang);
	int getAngCabeza();
	void dibujarTronco();
	void dibujarManoDer();
	void dibujarAntebDer();
	void dibujarBrazoDer();
	void dibujarManoIzq();
	void dibujarAntebIzq();
	void dibujarBrazoIzq();
	void dibujarPieDer();
	void dibujarPantDer();
	void dibujarPiernaDer();
	void dibujarPieIzq();
	void dibujarPantIzq();
	void dibujarPiernaIzq();
	void dibujarCabeza();
	void dibujar();
};