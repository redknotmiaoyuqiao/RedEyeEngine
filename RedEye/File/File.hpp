class ImageFile
{
public:
    ImageFile();
    ~ImageFile();

    unsigned char * ReadImage(const char * path,int * width,int * height,int * nrChannels);
};
