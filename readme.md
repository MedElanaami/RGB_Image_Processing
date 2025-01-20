# Traitement d'Images RGB en C

## Description
Ce projet implémente un système de traitement d'images RGB en C, permettant la création, l'initialisation et la manipulation d'images numériques. Le programme offre une interface utilisateur simple pour gérer des images RGB et appliquer des filtres basiques.

## Fonctionnalités
- Création d'images RGB avec dimensions personnalisables
- Deux modes d'initialisation :
  - Génération automatique avec des valeurs aléatoires
  - Saisie manuelle des valeurs RGB pour chaque pixel
- Traitement d'image :
  - Filtre de flou (moyenne 3x3)
- Affichage des valeurs RGB avant et après traitement

## Structure du Programme
Le programme est organisé autour d'une structure principale `RGBImage` qui contient :
- Un tableau 3D pour les données RGB
- Largeur de l'image
- Hauteur de l'image
- Résolution

## Comment Utiliser

### Compilation
```bash
gcc -o traitement_image main.c
```

### Exécution
```bash
./traitement_image
```

### Utilisation
1. Lancer le programme
2. Entrer les dimensions souhaitées pour l'image (largeur et hauteur)
3. Choisir le mode d'initialisation :
   - Option 1 : Valeurs aléatoires
   - Option 2 : Saisie manuelle des valeurs
4. Observer les valeurs de l'image avant traitement
5. Le programme applique automatiquement le filtre de flou
6. Observer les résultats après traitement

## Fonctions Principales

### `createImage`
- Crée une nouvelle image avec les dimensions spécifiées
- Alloue la mémoire nécessaire pour les données RGB

### `initializeRandom`
- Initialise l'image avec des valeurs RGB aléatoires
- Utilise la fonction rand() pour générer des valeurs entre 0 et 255

### `initializeFromKeyboard`
- Permet à l'utilisateur de saisir manuellement les valeurs RGB
- Vérifie que les valeurs sont comprises entre 0 et 255

### `applyBlur`
- Applique un filtre de flou sur l'image
- Utilise une moyenne 3x3 des pixels environnants

### `displayImage`
- Affiche les valeurs RGB de chaque pixel
- Format d'affichage : Pixel[x][y]: R:xxx G:xxx B:xxx

## Gestion de la Mémoire
- Allocation dynamique de la mémoire pour les données d'image
- Libération appropriée de la mémoire après utilisation
- Utilisation de tableaux temporaires pour le traitement d'image

## Limitations
- Taille d'image limitée par la mémoire disponible
- Filtre de flou simple (moyenne 3x3)
- Les bords de l'image ne sont pas traités par le filtre de flou

## Améliorations Possibles
1. Ajout de nouveaux filtres (Sobel, Gauss, etc.)
2. Support pour le chargement/sauvegarde d'images depuis/vers des fichiers
3. Interface graphique
4. Optimisation des performances pour les grandes images
5. Traitement des bords de l'image

## Notes Techniques
- Langage : C
- Utilisation intensive de pointeurs et d'allocation dynamique
- Structure de données optimisée pour le traitement d'images




--
[Previous sections remain the same...]

## Logique du Filtre de Flou (Blur)

### Principe de Base
Le filtre de flou implémente un "box blur" ou "mean blur" qui fonctionne selon le principe suivant :
- Pour chaque pixel de l'image, on calcule la moyenne des valeurs RGB des pixels voisins
- On utilise une fenêtre glissante de 3x3 pixels
- Le pixel central est remplacé par la moyenne des pixels dans la fenêtre

### Formule Mathématique
Pour chaque composante de couleur (R, G, B) :
```
Pixel_floué[x][y] = Σ(pixels_voisins) / nombre_de_pixels_voisins
```

### Étapes Détaillées de l'Algorithme
1. **Création d'une Image Temporaire**
   - Une copie de l'image originale est créée pour stocker les résultats
   - Ceci évite que les calculs soient affectés par les pixels déjà floutés

2. **Parcours de l'Image**
   ```
   Pour chaque pixel (i,j):
     Pour chaque composante couleur (R,G,B):
       somme = 0
       compte = 0
       Pour di = -1 à 1:
         Pour dj = -1 à 1:
           Si le pixel voisin (i+di, j+dj) est dans l'image:
             somme += valeur_du_pixel
             compte++
       nouvelle_valeur = somme / compte
   ```

3. **Gestion des Bords**
   - Les pixels en bordure utilisent moins de voisins
   - Le nombre de voisins (compte) est ajusté automatiquement
   - Exemple: un pixel dans un coin n'utilise que 4 pixels au lieu de 9

### Exemple Numérique
Pour un pixel central avec ses voisins :
```
Avant le flou :          Calcul :
[50  100  50]           (50 + 100 + 50 +
[100 200 100]  -->      100 + 200 + 100 +    -->  Résultat = 100
[50  100  50]           50 + 100 + 50) / 9
```

### Impact du Flou
- Réduit les variations brusques de couleur
- Adoucit les détails fins de l'image
- L'intensité du flou dépend du nombre de fois qu'il est appliqué

### Complexité
- Temporelle : O(w × h × 9) où w = largeur, h = hauteur
- Spatiale : O(w × h) pour l'image temporaire

[Rest of the README remains the same...]