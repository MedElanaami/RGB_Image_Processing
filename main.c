#include <stdio.h>
#include <stdlib.h>

typedef struct
{
  unsigned char ***data;
  int width;
  int height;
  int resolution;
} RGBImage;

// Function to create a new RGB image
RGBImage *createImage(int width, int height, int resolution)
{
  RGBImage *img = (RGBImage *)malloc(sizeof(RGBImage));
  img->width = width;
  img->height = height;
  img->resolution = resolution;

  // Allocate memory for image data
  img->data = (unsigned char ***)malloc(3 * sizeof(unsigned char **));
  for (int c = 0; c < 3; c++)
  {
    // Allocate memory for each color channel
    img->data[c] = (unsigned char **)malloc(height * sizeof(unsigned char *));
    for (int i = 0; i < height; i++)
    {
      img->data[c][i] = (unsigned char *)malloc(width * sizeof(unsigned char));
    }
  }

  return (RGBImage *)img;
}

// Function to initialize image with random values
void initializeRandom(RGBImage *img)
{
  // Seed the random number generator
  for (int c = 0; c < 3; c++)
  {
    for (int i = 0; i < img->height; i++)
    {
      for (int j = 0; j < img->width; j++)
      {
        img->data[c][i][j] = rand() % 256;
      }
    }
  }
}

// Function to initialize image from keyboard input
void initializeFromKeyboard(RGBImage *img)
{
  printf("\nEnter RGB values (0-255) for each pixel:\n");
  for (int i = 0; i < img->height; i++)
  {
    for (int j = 0; j < img->width; j++)
    {
      printf("Pixel [%d][%d] (R G B): ", i, j);
      for (int c = 0; c < 3; c++)
      {
        scanf("%hhu", &img->data[c][i][j]);
      }
      printf("\n");
    }
  }
}

// Function to display image values
void displayImage(RGBImage *img, const char *title)
{
  printf("\n%s:\n", title);
  for (int i = 0; i < img->height; i++)
  {
    for (int j = 0; j < img->width; j++)
    {
      printf("Pixel[%d][%d]: R:%3d G:%3d B:%3d | ",
             i, j,
             img->data[0][i][j],
             img->data[1][i][j],
             img->data[2][i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

// Basic blur filter implementation
void applyBlur(RGBImage *img)
{
  // Create temporary array to store blurred values
  unsigned char ***temp = (unsigned char ***)malloc(3 * sizeof(unsigned char **));
  for (int c = 0; c < 3; c++)
  {
    temp[c] = (unsigned char **)malloc(img->height * sizeof(unsigned char *));
    for (int i = 0; i < img->height; i++)
    {
      temp[c][i] = (unsigned char *)malloc(img->width * sizeof(unsigned char));
      // Copy original values to temp array
      for (int j = 0; j < img->width; j++)
      {
        temp[c][i][j] = img->data[c][i][j];
      }
    }
  }

  // Apply blur to all pixels except borders
  for (int i = 0; i < img->height; i++)
  {
    for (int j = 0; j < img->width; j++)
    {
      for (int c = 0; c < 3; c++)
      {
        int sum = 0;
        int count = 0;

        // Calculate average of surrounding pixels
        for (int di = -1; di <= 1; di++)
        {
          for (int dj = -1; dj <= 1; dj++)
          {
            int ni = i + di;
            int nj = j + dj;

            // Check if neighbor pixel is within bounds
            if (ni >= 0 && ni < img->height && nj >= 0 && nj < img->width)
            {
              sum += img->data[c][ni][nj];
              count++;
            }
          }
        }

        // Store average in temp array
        temp[c][i][j] = sum / count;
      }
    }
  }

  // Copy blurred result back to original image
  for (int c = 0; c < 3; c++)
  {
    for (int i = 0; i < img->height; i++)
    {
      for (int j = 0; j < img->width; j++)
      {
        img->data[c][i][j] = temp[c][i][j];
      }
    }
  }

  // Free temporary memory
  for (int c = 0; c < 3; c++)
  {
    for (int i = 0; i < img->height; i++)
    {
      free(temp[c][i]);
    }
    free(temp[c]);
  }
  free(temp);
}

void freeImage(RGBImage *img)
{
  for (int c = 0; c < 3; c++)
  {
    for (int i = 0; i < img->height; i++)
    {
      free(img->data[c][i]);
    }
    free(img->data[c]);
  }
  free(img->data);
  free(img);
}

int main()
{
  int choice;
  int width, height;

  printf("Image Processing Program\n");
  printf("Enter image dimensions:\n");
  printf("Width: ");
  scanf("%d", &width);
  printf("Height: ");
  scanf("%d", &height);

  RGBImage *img = createImage(width, height, 255);

  printf("\nChoose initialization method:\n");
  printf("1. Random values\n");
  printf("2. Enter values manually\n");
  printf("Enter choice (1 or 2): ");
  scanf("%d", &choice);

  switch (choice)
  {
  case 1:
    initializeRandom(img);
    break;
  case 2:
    initializeFromKeyboard(img);
    break;
  default:
    printf("Invalid choice. Using random values.\n");
    initializeRandom(img);
  }

  // Display original image
  displayImage(img, "Original Image");

  // Apply blur and display result
  printf("Applying blur filter...\n");
  applyBlur(img);
  displayImage(img, "Image After Blur");

  // Clean up
  freeImage(img);

  return 0;
}