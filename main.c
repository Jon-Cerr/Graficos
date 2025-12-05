/**
 * @file main.c
 * @author Alan Jonathan Cerritos Hernandez (alanchdez147@gmail.com)
 * @brief Juego Duck Hunt en C
 * @version 0.1
 * @date 2025-12-04
 *
 *
 */

#include "graficos.h"
#include <stdlib.h>
#include <time.h>
#define ALTURA_DEL_PASTO 600
#define VELOCIDAD_CAIDA 8
#define MIN_Y_APARICION 50
#define MAX_Y_APARICION 300

typedef struct
{
    int coorX;
    int coorY;
    int estado;
    int alto;
    int ancho;
    int velX;
    int velY;
    int totalPatos;
} Pato;

typedef struct
{
    bool rondaContinuar;
    int puntaje;
} Ronda;

void dibujarEscenarioRes1(Imagen *piso, Imagen *arbol);
void dibujarPato(Imagen *patoImg, Pato *pato);
void vueloPato(Pato *pato);
bool dispararPato(Pato *pato, Ronda *ronda);
void estadoPato(Imagen *patoImg, Pato *pato);
void gameLoop(Imagen *patoImg, Imagen *arbol, Imagen *piso, Pato *pato, Imagen *scoreImg, Ronda *ronda);

int main()
{
    srand(time(0));
    bool fs = false;
    Imagen *pisoImg = ventana.creaImagen("./piso.bmp");
    Imagen *arbolImg = ventana.creaImagen("./arbol.bmp");
    Imagen *patoImg = ventana.creaImagenConMascara("./pato1.bmp", "./patoMascara.bmp");
    Imagen *scoreImg = ventana.creaImagenConMascara("./score.bmp", "./scoreMascara.bmp");
    Pato *pato = (Pato *)malloc(sizeof(Pato));
    if (pato == NULL)
    {
        printf("El pato nacio morido :'/. Error en el programa...");
        ventana.imprimeEnConsola("El pato nacio morido :'/. Error en el programa...");
        return 1;
    }
    pato->coorX = 600;
    pato->coorY = 400;
    pato->estado = 0;
    pato->ancho = 100;
    pato->alto = 100;
    pato->velX = 5;
    pato->velY = -3;
    pato->totalPatos = 10;
    Ronda *ronda = (Ronda *)malloc(sizeof(Ronda));
    ronda->rondaContinuar = true;
    ronda->puntaje = 0;
    bool derecha_pres = false, izquierda_pres = false;
    ventana.tamanioVentana(1280, 720);
    ventana.tituloVentana("Duck Hunt");
    ventana.colorFondo(COLORES.AZULC);
    int tecla = ventana.teclaPresionada();
    while (tecla != TECLAS.ESCAPE && (ronda->rondaContinuar))
    {
        tecla = ventana.teclaPresionada();
        ventana.limpiaVentana(); // Limpiamos
        gameLoop(patoImg, arbolImg, pisoImg, pato, scoreImg, ronda);
        ventana.actualizaVentana(); // Mostramos
        ventana.espera(10);
    }
    ventana.cierraVentana();
    free(pato);
    return 0;
}

void dibujarEscenarioRes1(Imagen *piso, Imagen *arbol)
{
    ventana.muestraImagenEscalada(0, ventana.altoVentana() - 250, ventana.anchoVentana(), 350, piso);
    ventana.muestraImagenEscalada(100, ventana.altoVentana() - 570, 150, 350, arbol);
}

void dibujarPato(Imagen *patoImg, Pato *pato)
{
    ventana.muestraImagenEscalada(pato->coorX, pato->coorY, pato->ancho, pato->alto, patoImg);
}

void vueloPato(Pato *pato)
{
    if (pato->estado == 0)
    {
        pato->coorX += pato->velX;
        pato->coorY += pato->velY;
    }

    if (pato->coorY <= 0)
    {
        pato->velY *= -1;
    }

    if (pato->coorX <= 0 || (pato->coorX + pato->ancho) >= ventana.anchoVentana())
    {
        pato->velX *= -1;
    }

    if (pato->coorY >= (ventana.altoVentana() - 300))
    {
        pato->estado = 2;
    }
}

bool dispararPato(Pato *pato, Ronda *ronda)
{
    int rx, ry;
    bool botonPres = ventana.ratonBotonIzquierdo();
    ventana.raton(&rx, &ry);
    if (botonPres && pato->estado == 0)
    {
        if (rx >= pato->coorX &&
            rx <= (pato->coorX + pato->ancho))
        {
            if (ry >= pato->coorY &&
                ry <= (pato->coorY + pato->alto))
            {
                pato->estado = 1;
                pato->totalPatos--;
                ventana.muestraMensaje1("El pato ta' morido unu", "Le diste!");
                ronda->puntaje += 1000;
                if (pato->totalPatos == 0)
                {
                    ronda->rondaContinuar = ventana.muestraPregunta("Terminaste el juego, felicidades! :D. Continuar?");
                    if (ronda->rondaContinuar == false)
                    {
                        ventana.muestraMensaje("Hasta luego!");
                    }
                    else
                    {
                        pato->totalPatos = 10;
                        pato->estado = 2;
                    }
                }
                return false;
            }
        }
    }
}

void gameLoop(Imagen *patoImg, Imagen *arbolImg, Imagen *pisoImg, Pato *pato, Imagen *scoreImg, Ronda *ronda)
{
    dibujarEscenarioRes1(pisoImg, arbolImg);
    ventana.muestraImagenEscalada(150, ventana.altoVentana() - 200, 50, 50, scoreImg);

    vueloPato(pato);
    dispararPato(pato, ronda);
    estadoPato(patoImg, pato);
    if (pato->estado != 2)
    {
        dibujarPato(patoImg, pato);
    }
    ventana.color(COLORES.NEGRO);
    int rx, ry;
    ventana.raton(&rx, &ry);
    ventana.circuloRelleno(rx, ry, 10);
}

void estadoPato(Imagen *patoImg, Pato *pato)
{
    if (pato->estado == 1)
    {
        pato->coorY += VELOCIDAD_CAIDA;

        if (pato->coorY >= ALTURA_DEL_PASTO)
        {
            pato->estado = 2;
        }
    }
    else if (pato->estado == 2)
    {
        int maxCoorX = ventana.anchoVentana() - pato->ancho;
        pato->coorX = rand() % (maxCoorX + 1);
        pato->coorY = (rand() % (MAX_Y_APARICION - MIN_Y_APARICION + 1)) + MIN_Y_APARICION;
        pato->velX = (rand() % 5 + 4) * ((rand() % 2 == 0) ? 1 : -1);
        pato->velY = (rand() % 5) - 2;
        pato->estado = 0;
    }
}