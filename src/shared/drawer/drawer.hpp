#ifndef DMITRY_DERGACHEV_17_10_2008_DRAWER_H
#define DMITRY_DERGACHEV_17_10_2008_DRAWER_H

class drawer_t {
public:
	drawer_t();
	~drawer_t();
	void open();
private:
	static void* handle_;
};

#endif
