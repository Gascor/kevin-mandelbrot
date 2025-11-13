#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <complex.h>
#include <math.h>
#include "ppm.h"

#define TRSH 2.0
#define ITER 1024ull

#define SIZEX 1500
#define SIZEY 1500

/* Palette simple couleur */
static void color_from_iter(unsigned long int iter,
                            unsigned char *r,
                            unsigned char *g,
                            unsigned char *b)
{
    if (iter >= ITER) {
        *r = *g = *b = 0;
        return;
    }

    double t = (double)iter / (double)ITER;

    *r = (unsigned char)(9.0  * (1.0 - t) * t * t * t * 255.0);
    *g = (unsigned char)(15.0 * (1.0 - t) * (1.0 - t) * t * t * 255.0);
    *b = (unsigned char)(8.5 * (1.0 - t) * (1.0 - t) * (1.0 - t) * t * 255.0);
}

/* Rendu standard (grey ou color) */
static void render_frame(struct ppm_image *im, int use_color)
{
    int i, j;
    double colref = 255.0 / log((double)ITER);

    for (i = 0; i < SIZEX; i++) {
        for (j = 0; j < SIZEY; j++) {

            unsigned long iter = 0;
            double complex c = (-2.0 + (3.0 * i) / SIZEX)
                             + (-1.0 + (2.0 * j) / SIZEY) * I;
            double complex z = 0;

            while (iter < ITER) {
                if (cabs(z) > TRSH)
                    break;
                z = z * z + c;
                iter++;
            }

            unsigned char r, g, b;

            if (use_color) {
                color_from_iter(iter, &r, &g, &b);
            } else {
                int grey = (iter > 0) ? (int)(colref * log((double)iter)) : 0;
                if (grey < 0) grey = 0;
                if (grey > 255) grey = 255;
                r = g = b = grey;
            }

            ppm_image_setpixel(im, i, j, r, g, b);
        }
    }
}

/* Image simple */
static int render_single_image(int use_color, const char *out)
{
    struct ppm_image im;
    ppm_image_init(&im, SIZEX, SIZEY);

    render_frame(&im, use_color);

    ppm_image_dump(&im, (char *)out);
    ppm_image_release(&im);

    return 0;
}

/* GIF simple : on zoom un tout petit peu, on génère peu d'images */
static int render_gif(const char *gifname)
{
    int frames = 15;

    for (int k = 0; k < frames; k++) {

        /* petit déplacement pour créer un effet de mouvement */
        double zoom = 1.0 - 0.02 * k;

        struct ppm_image im;
        ppm_image_init(&im, SIZEX, SIZEY);

        for (int i = 0; i < SIZEX; i++) {
            for (int j = 0; j < SIZEY; j++) {
                unsigned long iter = 0;

                double x = (-2.0 + (3.0 * i) / SIZEX) * zoom;
                double y = (-1.0 + (2.0 * j) / SIZEY) * zoom;

                double complex c = x + y * I;
                double complex z = 0;

                while (iter < ITER) {
                    if (cabs(z) > TRSH) break;
                    z = z * z + c;
                    iter++;
                }

                unsigned char r, g, b;
                color_from_iter(iter, &r, &g, &b);

                ppm_image_setpixel(&im, i, j, r, g, b);
            }
        }

        char name[64];
        snprintf(name, sizeof(name), "m_%03d.ppm", k);
        ppm_image_dump(&im, name);
        ppm_image_release(&im);
    }

    /* Convertir en GIF */
    char cmd[256];
    snprintf(cmd, sizeof(cmd),
             "convert -delay 8 -loop 0 m_*.ppm %s",
             gifname);

    int ret = system(cmd);
    if (ret != 0) {
        fprintf(stderr, "Erreur: ImageMagick convert n'est pas installé.\n");
        return 1;
    }

    system("rm -f m_*.ppm");
    return 0;
}

static void usage(const char *p)
{
    printf("Usage:\n");
    printf("  %s grey [out.ppm]\n", p);
    printf("  %s color [out.ppm]\n", p);
    printf("  %s gif [out.gif]\n", p);
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        usage(argv[0]);
        return 1;
    }

    const char *mode = argv[1];
    const char *out  = (argc > 2) ? argv[2] : NULL;

    if (!out) {
        if (strcmp(mode, "gif") == 0) out = "zoom.gif";
        else out = "m.ppm";
    }

    if (strcmp(mode, "grey") == 0 || strcmp(mode, "gray") == 0)
        return render_single_image(0, out);

    if (strcmp(mode, "color") == 0)
        return render_single_image(1, out);

    if (strcmp(mode, "gif") == 0)
        return render_gif(out);

    usage(argv[0]);
    return 1;
}
