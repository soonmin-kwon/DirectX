#ifndef _TEST_H_
#define _TEST_H_

template <typename T>
void Test(T* a) {
	if (a) {
		a->Release();
		a = 0;
	}
}

#endif // !TEST_H_

