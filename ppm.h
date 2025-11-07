/*  ppm.h — Mini-API d’écriture d’images PPM (format P6)
 *
 *  But
 *    Fournir des primitives simples pour créer une image RGB en mémoire
 *    et l’écrire sur disque au format P6 (binaire).
 *
 *  Structures
 *    struct ppm_pixel { unsigned char r,g,b; };
 *    struct ppm_image { unsigned int width, height; struct ppm_pixel *px; };
 *
 *  Fonctions
 *    int  ppm_image_init(struct ppm_image *im, int w, int h);
 *         // alloue im->px (w*h pixels) et initialise width/height.
 *    void ppm_image_setpixel(struct ppm_image *im,int x,int y,
 *                            unsigned char r,unsigned char g,unsigned char b);
 *         // écrit un pixel (0 <= x < width, 0 <= y < height).
 *    int  ppm_image_dump(struct ppm_image *im, char *path);
 *         // écrit le fichier PPM au format:
 *         //   P6\n<width> <height>\n255\n + données RGB (haut->bas, gauche->droite).
 *    int  ppm_image_release(struct ppm_image *im);
 *         // libère la mémoire allouée pour im->px.
 *
 *  Notes
 *    - Les composantes r/g/b sont des octets [0..255].
 *    - L’appelant doit respecter les bornes x/y.
 *    - Retour 0 en cas de succès (convention habituelle), valeur non nulle sinon.
 */

#ifndef PPM_H
#define PPM_H


struct ppm_pixel {
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

static inline void ppm_setpixel(
		struct ppm_pixel *px,
		unsigned char r, unsigned char g, unsigned char b)
{
	px->r = r;
	px->g = g;
	px->b = b;
}

struct ppm_image {
	unsigned int width;
	unsigned int height;
	struct ppm_pixel *px;
};

int ppm_image_init(struct ppm_image *im, int w, int h);
int ppm_image_release(struct ppm_image *im);

static inline void ppm_image_setpixel(
		struct ppm_image *im, int x, int y,
		unsigned char r, unsigned char g, unsigned char b)
{
	struct ppm_pixel *px = im->px + im->width * y + x;
	ppm_setpixel(px, r, g, b);
}

int ppm_image_dump(struct ppm_image *im, char *path);


#endif // PPM_H
