#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "utils.h"
#include "event.h"
#include "data.h"
#include "text.h"
#include "game.h"

void intro(SDL_Window *window, SDL_Renderer *renderer, Input *in, Pictures *pictures, Fonts *fonts, Sounds *sounds, FPSmanager *fps)
{
    if(!textIntro(window, renderer, in, pictures, fonts, sounds, fps, "C'est sur le plateau verdoyant de la r�gion des lagunes,$dans la petite bourgade de Cocody rattach�e � Abidjan que$naquit Zongo le Dozo en 1980. V�ritable force de la nature,$nourri au terroir tumultueux de cette r�gion sauvage$pendant 3 ann�es, il se voit forc� de quitter la terre$originelle des Hommes pour venir dans l'enfer de la banlieue$parisienne."))
        return;
    if(!textIntro(window, renderer, in, pictures, fonts, sounds, fps, "Fort d'un baccalaur�at STT, poursuivant des �tudes$scientifiques � la factult� du val d'Oise, il apprend$l'anatomie pour parfaire en secret des techniques de$neutralisation l�thales."))
        return;
    if(!textIntro(window, renderer, in, pictures, fonts, sounds, fps, "Gladiateur des temps modernes, montagne de testost�rone,$il n'a fallu que quelques ann�es pour forger la r�putation$de cet �tre surhumain qui a gagn� � la loterie g�n�tique.$Zongo presse le pas, et commence � s'interroger sur son$avenir : s'il ne pouvait pas gravir les �chelons de la$vieille Europe en plein �moi, il allait s'autoproclamer Roi,$et rev�tir un nouveau blason qui enverrait un message fort de$puissance. Il optera ainsi pour le pseudonyme \"Kaaris\" qui$signifie \"Roi des hommes\" en Ivoirien."))
        return;
    if(!textIntro(window, renderer, in, pictures, fonts, sounds, fps, "Invaincu en 78 combats men�s clandestinement pour lesquels$il ne laisse aucune trace num�rique par fiert�, il d�cide �$contrecoeur d'accepter la proposition d'une exhibition$martiale d'un troubadour local. Et c'est en D�cembre qu'il$r�v�lera au monde sa vraie nature, � coup de salades de$phalanges savamment plac�es par un tour de bras de 98 cm.$Personne en ce monde n'est pr�t pour tel spectacle."))
        return;
    if(!textIntro(window, renderer, in, pictures, fonts, sounds, fps, "Zongo le Dozo va r�aliser son r�ve, et son ennemi n'aura$le choix que de go�ter � la subtile nuance sal�e de son$propre sang parfum� par la rage de Cocody."))
        return;
}





int textIntro(SDL_Window *window, SDL_Renderer *renderer, Input *in, Pictures *pictures, Fonts *fonts, Sounds *sounds, FPSmanager *fps, char *str)
{
    SDL_Color white = {255, 255, 255, 255};
    int escape = 0;
    int current_max = 0;
    int len = strlen(str);
    int total_num_lines = 0;
    SDL_Rect pos_fs;
    pos_fs.x = 0;
    pos_fs.y = 0;
    pos_fs.w = WINDOW_W;
    pos_fs.h = WINDOW_H;

    // Compute number of lines in str
    for(int i = 0; str[i] != '\0'; i++)
        if(str[i] == '$')
            total_num_lines++;


    while(!escape)
    {
        updateEvents(in);

        if(in->quit)
            exit(EXIT_SUCCESS);
        if(KEY_ESCAPE || in->controller[0].buttons[7] || in->controller[1].buttons[7])
        {
            in->key[SDL_SCANCODE_ESCAPE] = 0;
            in->controller[0].buttons[6] = 0;
            in->controller[0].buttons[7] = 0;
            in->controller[1].buttons[6] = 0;
            in->controller[1].buttons[7] = 0;

            return 0;
        }
        if(KEY_ENTER_MENU)
        {
            in->key[SDL_SCANCODE_SPACE] = 0;
            in->key[SDL_SCANCODE_RETURN] = 0;
            in->key[SDL_SCANCODE_KP_ENTER] = 0;
            in->controller[0].buttons[0] = 0;
            in->controller[1].buttons[0] = 0;


            if(current_max < len)
                current_max = len;
            else
                escape = 1;
        }


        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, pictures->title, NULL, &pos_fs);

        int index = 0;
        int line_num = 0;
        int offset = 0;

        while(index < current_max)
        {
            int line_size = 0;
            offset = index;

            while(index < current_max && str[index] != '\0' && str[index] != '$')
            {
                index++;
                line_size++;
            }

            int temp_index = index;
            int temp_line_size = line_size;

            while(str[temp_index] != '\0' && str[temp_index] != '$')
            {
                temp_index++;
                temp_line_size++;
            }

            index++;
            line_size++;
            temp_index++;
            temp_line_size++;

            char *line = xmalloc(line_size, window);
            for(int i = 0; i < line_size; i++)
                line[i] = str[offset + i];
            line[line_size - 1] = '\0';

            char *temp_line = xmalloc(temp_line_size, window);
            for(int i = 0; i < temp_line_size; i++)
                temp_line[i] = str[offset + i];
            temp_line[temp_line_size - 1] = '\0';

            SDL_Texture *texture = RenderTextBlended(renderer, fonts->preview_intro, line, white);
            SDL_Surface *temp_surface = TTF_RenderText_Blended(fonts->preview_intro, temp_line, white);

            free(line);
            free(temp_line);

            SDL_Rect pos_dst;
            SDL_QueryTexture(texture, NULL, NULL, &pos_dst.w, &pos_dst.h);
            pos_dst.x = WINDOW_W / 2 - temp_surface->w / 2;
            pos_dst.y = WINDOW_H / 2 - pos_dst.h / 2 - total_num_lines * 25 + line_num * 50 - 25;
            SDL_RenderCopy(renderer, texture, NULL, &pos_dst);
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(temp_surface);

            line_num++;
        }


        if(index < len && str[index] == '$')
            Mix_PlayChannel(-1, sounds->linefeed, 0);

        SDL_RenderPresent(renderer);
        SDL_framerateDelay(fps);

        if(current_max < len)
            current_max++;
    }

    return 1;
}


