#include "tregla.h"



TClausuraItem::TClausuraItem(const TClausuraItem &claus):
    punto(claus.punto), id(claus.id),
    noTerm(claus.noTerm),
    produccion(claus.produccion){}

TClausuraItem::~TClausuraItem()
{
    produccion.clear();
}



TMetaSimbolo::~TMetaSimbolo()
{
    sim.clear();
}
