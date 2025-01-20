#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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
  printf("\n%s\n", title);
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

// Fonction utilitaire pour trier un tableau (utilise pour le filtre median)
void bubbleSort(unsigned char arr[], int n)
{
  for (int i = 0; i < n - 1; i++)
  {
    for (int j = 0; j < n - i - 1; j++)
    {
      if (arr[j] > arr[j + 1])
      {
        unsigned char temp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = temp;
      }
    }
  }
}

// Fonction de reduction du bruit avec filtre median
void reduceNoise(RGBImage *img)
{
  unsigned char ***temp = (unsigned char ***)malloc(3 * sizeof(unsigned char **));
  for (int c = 0; c < 3; c++)
  {
    temp[c] = (unsigned char **)malloc(img->height * sizeof(unsigned char *));
    for (int i = 0; i < img->height; i++)
    {
      temp[c][i] = (unsigned char *)malloc(img->width * sizeof(unsigned char));
    }
  }

  // Tableau pour stocker les valeurs des voisins
  unsigned char neighbors[9];

  for (int c = 0; c < 3; c++)
  {
    for (int i = 0; i < img->height; i++)
    {
      for (int j = 0; j < img->width; j++)
      {
        int idx = 0;

        // Collecter les valeurs des voisins
        for (int di = -1; di <= 1; di++)
        {
          for (int dj = -1; dj <= 1; dj++)
          {
            int ni = i + di;
            int nj = j + dj;

            if (ni >= 0 && ni < img->height && nj >= 0 && nj < img->width)
            {
              neighbors[idx++] = img->data[c][ni][nj];
            }
          }
        }

        // Trier et prendre la valeur mediane
        bubbleSort(neighbors, idx);
        temp[c][i][j] = neighbors[idx / 2];
      }
    }
  }

  // Copier le resultat
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

  // Liberer la memoire
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

// Fonction de detection des contours (Sobel)
RGBImage *detectEdges(RGBImage *img)
{
  // Creer une nouvelle image pour les contours
  RGBImage *edges = createImage(img->width, img->height, img->resolution);

  // Kernels de Sobel
  int sobelX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
  int sobelY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

  for (int c = 0; c < 3; c++)
  {
    for (int i = 1; i < img->height - 1; i++)
    {
      for (int j = 1; j < img->width - 1; j++)
      {
        int gx = 0, gy = 0;

        // Appliquer les kernels de Sobel
        for (int di = -1; di <= 1; di++)
        {
          for (int dj = -1; dj <= 1; dj++)
          {
            int pixel = img->data[c][i + di][j + dj];
            gx += pixel * sobelX[di + 1][dj + 1];
            gy += pixel * sobelY[di + 1][dj + 1];
          }
        }

        // Calculer le gradient
        int magnitude = (int)sqrt(gx * gx + gy * gy);
        // Normaliser et limiter à 255
        magnitude = magnitude > 255 ? 255 : magnitude;
        edges->data[c][i][j] = (unsigned char)magnitude;
      }
    }
  }

  return edges;
}

// Mise à jour du menu principal
int main()
{
  int choice, operation;
  int width, height;
  printf("\033[42mProgramme de Traitement d'Images\033[0m\n");
  printf("Entrez les dimensions de l'image:\n");
  printf("Largeur: ");
  scanf("%d", &width);
  printf("Hauteur: ");
  scanf("%d", &height);

  RGBImage *img = createImage(width, height, 255);

  printf("\n\033[43mChoisissez la methode d'initialisation:\033[0m\n");
  printf("1. Valeurs aleatoires\n");
  printf("2. Saisie manuelle\n");
  printf("Entrez votre choix (1 ou 2): ");
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
    printf("\033[41mChoix invalide. Utilisation de valeurs aleatoires.\033[0m\n");
    initializeRandom(img);
  }

  do
  {
    printf("\n\033[43mOperations disponibles:\033[0m\n");
    printf("1. Afficher l'image\n");
    printf("2. Appliquer le flou\n");
    printf("3. Reduire le bruit\n");
    printf("4. Detecter les contours\n");
    printf("0. Quitter\n");
    printf("Choisissez une operation: ");
    scanf("%d", &operation);

    switch (operation)
    {
    case 1:
      displayImage(img, "\033[44mImage actuelle\033[0m");
      break;
    case 2:
      applyBlur(img);
      printf("\033[44mFlou applique.\033[0m\n");
      displayImage(img, "\033[44mImage apres flou\033[0m");
      break;
    case 3:
      reduceNoise(img);
      printf("\033[44mReduction du bruit effectuee.\033[0m\n");
      displayImage(img, "\033[44mImage apres reduir le bruit\033[0m");
      break;
    case 4:
    {
      RGBImage *edges = detectEdges(img);
      displayImage(edges, "\033[44mContours detectes\033[0m");
      freeImage(edges);
      break;
    }
    case 0:
      printf("\033[42mAu revoir!\033[0m\n");
      break;
    default:
      printf("\033[41mOperation invalide!\033[0m\n");
    }
  } while (operation != 0);

  freeImage(img);
  return 0;
}