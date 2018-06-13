#include "itkMetaImageIO.h"

#if defined(__EMSCRIPTEN__)
#include <emscripten.h>
#endif

int main( int argc, char * argv[] )
{
  if (argc < 2)
    {
    std::cerr << "Usage: " << argv[0] << " orientedImage.mha" << std::endl;
    return 1;
    }
  const char * inputFileName = argv[1];

#if defined(__EMSCRIPTEN__)
  EM_ASM(
    source_mount_dir = '/work/itk-js-decompression';
    FS.mkdir('/work');
    FS.mkdir(source_mount_dir);
    FS.mount(NODEFS, { root: source_mount_dir }, source_mount_dir );
  );
#endif
  itk::MetaImageIO::Pointer imageIO = itk::MetaImageIO::New();
  imageIO->SetFileName(inputFileName);
  std::cout << "Can read file: " << imageIO->CanReadFile(inputFileName) << std::endl;
  if(!imageIO->CanReadFile(inputFileName))
    {
    return 1;
    }

  imageIO->ReadImageInformation();

  const unsigned int dimensions = imageIO->GetNumberOfDimensions();
  std::cout << "Number of dimensions: " << imageIO->GetNumberOfDimensions() << std::endl;
  std::cout << "IO component type: " << imageIO->GetComponentType() << std::endl;
  std::cout << "IO Pixel type: " << imageIO->GetPixelType() << std::endl;
  std::cout << "Components: " << imageIO->GetNumberOfComponents() << std::endl;
  const unsigned long imageSizeInBytes = imageIO->GetImageSizeInBytes();
  std::cout << "Image size in bytes: " << imageSizeInBytes << std::endl;

  itk::ImageIORegion ioRegion( dimensions );
  for( unsigned int dim = 0; dim < dimensions; ++dim )
    {
    ioRegion.SetSize( dim, imageIO->GetDimensions( dim ) );
    }
  std::cout << "ioRegion: " << ioRegion << std::endl;
  imageIO->SetIORegion( ioRegion );

  char * loadBuffer = new char[imageSizeInBytes];
  imageIO->Read( static_cast< void * >( loadBuffer ) );

  std::cout << "Buffer contents: " << std::endl;
  for( unsigned int ii = 5000; ii < 6000; ++ii )
    {
    std::cout << static_cast< int >( loadBuffer[ii] );
    }
  std::cout << std::endl;

  delete[] loadBuffer;

#if defined(__EMSCRIPTEN__)
  EM_ASM(
    FS.unmount(source_mount_dir);
  );
#endif
  return 0;
}
