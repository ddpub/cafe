#include "logo_maker.hpp"
#include <ksi_include/ksi_exceptions.hpp>

std::vector<char> from_bmp(const std::vector<unsigned char>& source, size_t width, size_t height)
{
	if (source.size() % 3) THROW_MESSAGE(0, "invalid size bitmap array", "std::vector<char> from_bmp(const std::vector<unsigned char>&)")
	size_t pixel_count = source.size()/3;
	if (pixel_count != width*height) THROW_MESSAGE(1, "invalid size bitmap array", "std::vector<char> from_bmp(const std::vector<unsigned char>&)")
	
	std::vector<char> data;
	
	size_t mx, my;
	{
		size_t dx = width%8 ? 8-width%8 : 0;
		size_t dy = height%8 ? 8-height%8 : 0;
		mx = (width+dx)/8;
		my = (height+dy)/8;
		data.push_back(int(mx-(mx/256)*256));
		data.push_back(int(mx/256));
		data.push_back(int(my-(my/256)*256));
		data.push_back(int(my/256));
	}
	
	std::vector<unsigned char> msource;
	for (size_t i=0; i<width; ++i)
		for (size_t j=0; j<height; ++j)
	{
		msource.push_back(source[j*width*3+i*3]);
		msource.push_back(source[j*width*3+i*3+1]);
		msource.push_back(source[j*width*3+i*3+2]);
	}

	for (size_t i=0; i<width; ++i)
	{
		char val = 0;
		int val_count = 0;
		for (size_t j=0; j<height; ++j)
		{
			unsigned char r = msource[i*height*3+j*3];
			unsigned char g = msource[i*height*3+j*3+1];
			unsigned char b = msource[i*height*3+j*3+2];
			unsigned char bit = 0; //whilte
			if (r+g+b/3 < 127) bit = 1;
			if (val_count == 8)
			{
				data.push_back(val);
				val = 0;
				val_count = 0;
			}
			val = val << 1;
			val = val | bit;
			val_count++;
		}
		if (val_count == 8)
		{
			data.push_back(val);
			val = 8;
			val_count = 0;
		}
		else if (val_count !=8)
		{
			val = val << 8-val_count;
			data.push_back(val);
		}
	}
	for (size_t i=data.size(); i<mx*my*8+4; ++i) data.push_back(0);
	data.push_back(0x1);
	return data;
}

std::vector<unsigned char> to_bmp(const std::vector<char>& source, size_t& width, size_t& height)
{
	if (source.size() < 12) THROW_MESSAGE(0, "invalid size bitmap array", "std::vector<unsigned char> to_bmp(const std::vector<char>&, size_t&, size_t&)")
	size_t bx = source[0]+source[1]*256;
	size_t by = source[2]+source[3]*256;
	if (source.size() != 4+bx*by*8+1) THROW_MESSAGE(2, "invalid size bitmap array", "std::vector<unsigned char> to_bmp(const std::vector<char>&, size_t&, size_t&)")
	std::vector<unsigned char> data;
	for (size_t i=4; i<source.size(); ++i)
	{
		if (source[i] & 128) { data.push_back(0); data.push_back(0); data.push_back(0); } else { data.push_back(255); data.push_back(255); data.push_back(255); }
		if (source[i] & 64) { data.push_back(0); data.push_back(0); data.push_back(0); } else { data.push_back(255); data.push_back(255); data.push_back(255); }
		if (source[i] & 32) { data.push_back(0); data.push_back(0); data.push_back(0); } else { data.push_back(255); data.push_back(255); data.push_back(255); }
		if (source[i] & 16) { data.push_back(0); data.push_back(0); data.push_back(0); } else { data.push_back(255); data.push_back(255); data.push_back(255); }
		if (source[i] & 8) { data.push_back(0); data.push_back(0); data.push_back(0); } else { data.push_back(255); data.push_back(255); data.push_back(255); }
		if (source[i] & 4) { data.push_back(0); data.push_back(0); data.push_back(0); } else { data.push_back(255); data.push_back(255); data.push_back(255); }
		if (source[i] & 2) { data.push_back(0); data.push_back(0); data.push_back(0); } else { data.push_back(255); data.push_back(255); data.push_back(255); }
		if (source[i] & 1) { data.push_back(0); data.push_back(0); data.push_back(0); } else { data.push_back(255); data.push_back(255); data.push_back(255); }
	}
	std::vector<unsigned char> data2;

	width = bx*8;
	height = by*8;
	
	for (size_t i=0; i<height; ++i)
	{
		for (size_t j=0; j<width; ++j)
		{			
			data2.push_back(data[j*height*3+i*3]);
			data2.push_back(data[j*height*3+i*3+1]);
			data2.push_back(data[j*height*3+i*3+2]);
		}
	}
	return data2;
}
