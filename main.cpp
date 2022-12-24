#include <iostream>
#include <string>
#include <fstream>
//#pragma warning(disable : 4996)
#include <array>
#include <vector>
#include <iterator>

#include <omp.h>


static constexpr size_t HEADER_SIZE = 54;
class BMPHeader : public std::array<char, HEADER_SIZE> 
{
public:
	uint32_t file_size() const { return get_value<uint32_t, 2>(); }
	uint32_t data_offset() const { return get_value<uint32_t, 10>(); }
	uint32_t width() const { return get_value<uint32_t, 18>(); }
	uint32_t height() const { return get_value<uint32_t, 22>(); }
	uint32_t depth() const { return get_value<uint16_t, 28>(); }
protected:
	template<class TYPE, size_t POS>
	TYPE get_value() const { return *reinterpret_cast<const TYPE*>(&(this->at(POS))); }
};


class BMPFile
{
public:
	explicit BMPFile(std::string file)
	{
		std::ifstream bmp(file, std::ios::binary);

    		bmp.read(m_header.data(), static_cast<long>(m_header.size()));

    		std::cout << "fileSize: " << m_header.file_size() << std::endl;
    		std::cout << "dataOffset: " << m_header.data_offset() << std::endl;
    		std::cout << "width: " << m_header.width() << std::endl;
    		std::cout << "height: " << m_header.height() << std::endl;
    		std::cout << "depth: " << m_header.depth() << "-bit" << std::endl;

    		//m_img.resize(m_header.data_offset() - m_header.size());
    		//bmp.read(m_img.data(), static_cast<long>(m_img.size()));

    		//size_t data_size = ((m_header.width() * 3 + 3) & (~static_cast<unsigned int>(3))) * m_header.height();
    		size_t data_size = m_header.width() * 3 * m_header.height();
    		m_img.resize(data_size);
    		bmp.read(m_img.data(), static_cast<long>(m_img.size()));

#if 0
    		char temp = 0;
		//int чтоб мог быть меньше 0
    		for (int i = static_cast<int>(data_size) - 4; i >= 0; i -= 3) {
			unsigned int ui = static_cast<unsigned int>(i);
        		temp = m_img[ui];
        		m_img[ui] = m_img[ui+2];
        		m_img[ui+2] = temp;
		
        		//std::cout << "R: " << int(m_img[ui] & 0xff) << " G: " << int(m_img[ui+1] & 0xff) << " B: " << int(m_img[ui+2] & 0xff) << std::endl;
    		}
#endif
		
	}
	
	class Color : public std::array<char, 3>
	{
	public:
		Color(char g, char b, char r) : std::array<char, 3>({r, g, b}) {}
		size_t r() const { return static_cast<size_t>((*this)[0]) & 0xff; }
		size_t g() const { return static_cast<size_t>((*this)[1]) & 0xff; }
		size_t b() const { return static_cast<size_t>((*this)[2]) & 0xff; }
		friend std::ostream& operator<<(std::ostream& stream, Color color)
		{
        		stream << "R: " << color.r() << " G: " << color.g() << " B: " << color.b();
			return stream;
		}
	};

	Color get_color(size_t x, size_t y) const 
	{
		size_t pos = (x + m_header.width() * y) * 3;
		return Color(m_img[pos], m_img[pos+1], m_img[pos+2]);
	}

	const BMPHeader& header() const { return m_header; }
	
protected:
	BMPHeader m_header;
	std::vector<char> m_img;
};

#if 0
std::vector<char> read_BMP(const std::string &file)
{

    std::ifstream bmp(file, std::ios::binary);

    std::array<char, HEADER_SIZE> header;
    bmp.read(header.data(), header.size());

    auto fileSize = *reinterpret_cast<uint32_t *>(&header[2]);
    auto dataOffset = *reinterpret_cast<uint32_t *>(&header[10]);
    auto width = *reinterpret_cast<uint32_t *>(&header[18]);
    auto height = *reinterpret_cast<uint32_t *>(&header[22]);
    auto depth = *reinterpret_cast<uint16_t *>(&header[28]);

    std::cout << "fileSize: " << fileSize << std::endl;
    std::cout << "dataOffset: " << dataOffset << std::endl;
    std::cout << "width: " << width << std::endl;
    std::cout << "height: " << height << std::endl;
    std::cout << "depth: " << depth << "-bit" << std::endl;

    std::vector<char> img(dataOffset - HEADER_SIZE);
    bmp.read(img.data(), static_cast<long>(img.size()));

    size_t dataSize = ((width * 3 + 3) & (~static_cast<unsigned int>(3))) * height;
    img.resize(dataSize);
    bmp.read(img.data(), static_cast<long>(img.size()));

    char temp = 0;

    for (int i = static_cast<int>(dataSize) - 4; i >= 0; i -= 3)
    {
        temp = img[i];
        img[i] = img[i+2];
        img[i+2] = temp;

        std::cout << "R: " << int(img[i] & 0xff) << " G: " << int(img[i+1] & 0xff) << " B: " << int(img[i+2] & 0xff) << std::endl;
    }

    return img;
}
#endif


int main(int argc, char** argv)
{

    if (argc < 2)
	    return 0;
    setlocale(LC_ALL, "Russian");
    for (int file_num = 1; file_num < argc; file_num++) {
	std::string file_name(argv[file_num]);
	//auto res = read_BMP(file_name);
	//std::cout << res.size() << std::endl;
	BMPFile bmp(file_name);
	for (size_t x = 0; x < bmp.header().width(); x++)
		for (size_t y = 0; y < bmp.header().height(); y++)
			std::cout << "[" << x << ", " << y << "] = " << bmp.get_color(x, y) << std::endl;
    }
    return 0;

#if 0
    for (int file_num = 1; file_num < argc; file_num++) {
    		//Define number of threads from task: 4 + (514+24) % 5 = 4 + 3 = 7
    		const int g_nNumberOfThreads = 7;
    		unsigned int th_id;

    		WAV_HEADER Header;
    		FILE* File;

    		File = fopen(argv[file_num], "rb");

    		//Reading the file header
    		fread(&Header, sizeof(WAV_HEADER), 1, File);
	
    		//Counting the number and dimension of samples
    		int sample_size = Header.bitsPerSample / 8;
    		unsigned long samples_count = Header.subchunk2Size * 8 / Header.bitsPerSample;

    		//Creating an array for writing a file
    		short int* Data = new short int[samples_count];

    		omp_set_num_threads(g_nNumberOfThreads);
		
    		//Launch parallel block
    		#pragma omp parallel private (th_id)
    		{
        		th_id = static_cast<unsigned int>(omp_get_thread_num());
		
        		//Define method from task: (514+24) % 4 = 2 -> method C
        		for (unsigned long i = (((th_id + 1) * samples_count) / g_nNumberOfThreads) - 1; i >= (th_id * samples_count) / g_nNumberOfThreads; i--)
        		{
            			fread(&Data[i], static_cast<std::size_t>(sample_size), 1, File);
        		}
        		#pragma omp barrier 
    		}

    		//Output of the first 50 values to compare correctness with Cool Edit
    		for (int i = 0; i < 50; i++)
    		{
        		cout << i << " element: " << Data[i] << "\n";
    		}

    		fclose(File);
    		(void)getchar();
	}
    	return 0;
#endif
}
