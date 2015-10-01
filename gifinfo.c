#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "gif_lib.h"

int main(int argc, char *argv[]) {
    if (argc < 2)
        errx(1, "Usage: %s FILE", argv[0]);

    int gif_err = 0;

    fprintf(stderr, "Opening file: %s\n", argv[1]);

    GifFileType *gif = DGifOpenFileName(argv[1], &gif_err);
    if (!gif)
        errx(1, "DGifOpenFileName: %s\n", GifErrorString(gif_err));

    fprintf(stderr, "Loading file\n");
    if (DGifSlurp(gif) == GIF_ERROR)
        errx(1, "DGifSlurp: %s\n", GifErrorString(gif->Error));

    printf("Dimensions: %i x %i px\n", gif->SWidth, gif->SHeight);
    printf("Color Resolution: %i bits\n", gif->SColorResolution);
    printf("Background Color: %i\n", gif->SBackGroundColor);
    int frame_count = gif->ImageCount;
    printf("Frame Count: %i\n", frame_count);

    ColorMapObject *global_color_map = gif->SColorMap;
    if (!global_color_map)
        printf("Global Color Map: absent\n");
    else {
        printf("Global Color Map:\n");
        int global_color_map_count = global_color_map->ColorCount;
        printf("  Count: %i\n", global_color_map_count);
        printf("  Depth: %i\n", global_color_map->BitsPerPixel);
        printf("  Colors:\n");

        for (int i = 0; i < global_color_map_count; ++i) {
            GifColorType color = global_color_map->Colors[i];
            printf("    Color %i: #%02x%02x%02x\n", i,
                   color.Red, color.Green, color.Blue);
        }
    }

    SavedImage *frames = gif->SavedImages;
    printf("Frames:\n");
    for (int i = 0; i < frame_count; ++i) {
        SavedImage frame = frames[i];
        printf("  Frame %i:\n", i);
        GifImageDesc frame_desc = frame.ImageDesc;
        printf("    Frame Interlaced: %s\n", frame_desc.Interlace ? "Yes" : "No");
        printf("    Frame Left: %i\n", frame_desc.Left);
        printf("    Frame Top: %i\n", frame_desc.Top);
        printf("    Frame Width: %i\n", frame_desc.Width);
        printf("    Frame Height: %i\n", frame_desc.Height);
        ColorMapObject *local_color_map = frame_desc.ColorMap;
        if (!local_color_map)
            printf("    Local Color Map: absent\n");
        else {
            printf("    Local Color Map:\n");
            int local_color_map_count = local_color_map->ColorCount;
            printf("      Count: %i\n", local_color_map_count);
            printf("      Depth: %i\n", local_color_map->BitsPerPixel);
            printf("      Colors:\n");

            for (int i = 0; i < local_color_map_count; ++i) {
                GifColorType color = local_color_map->Colors[i];
                printf("        Color %i: #%02x%02x%02x\n", i,
                       color.Red, color.Green, color.Blue);
            }
        }
    }

    fprintf(stderr, "Closing file\n");
    if (DGifCloseFile(gif, &gif_err) == GIF_ERROR)
        errx(1, "DGifCloseFile: %s\n", GifErrorString(gif_err));

    exit(0);
}
