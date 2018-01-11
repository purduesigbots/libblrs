#include "fbc_bangbang.h"

__attribute__((weak)) int sgn(int in) {
	return ((in > 0) - (in < 0));
}

static int _bangbangCompute(fbc_t* fbc, int error) {
	fbc_bangbang_t* data = (fbc_bangbang_t*)(fbc->_controllerData);

	if (sgn(error) != sgn(fbc->goal))
		data->_passed = true;

	if (data->_passed)
		return (data->kBrake * error);
	else
		return (sgn(fbc->goal) * data->fullSpeed);
}

static void _bangbangReset(fbc_t* fbc) {
	fbc_bangbang_t* data = (fbc_bangbang_t*)(fbc->_controllerData);
	data->_passed = false;
}

void fbcBangBangInitializeData(fbc_bangbang_t* bangbang, double kBrake, int fullSpeed) {
	bangbang->kBrake = kBrake;
	bangbang->fullSpeed = fullSpeed;
	bangbang->_passed = false;
}

void fbcBangBangInit(fbc_t* fbc, fbc_bangbang_t* config) {
	fbc->compute = &_bangbangCompute;
	fbc->_controllerData = config;
	fbc->resetController = &_bangbangReset;
}
