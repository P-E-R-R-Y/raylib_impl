/**
 * @file raylib.cpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief Les points d'entree dlsym de ce vendor.
 * @date 2026-08-10
 *
 * @addtogroup raylib
 * @{
 */

#ifndef RAYLIB_CPP_
#define RAYLIB_CPP_

#include "RayAudioModule.hpp"
#include "RayGraphicModule.hpp"

/*
 * Ces trois fonctions sont la SEULE surface que le chargeur voit. Elles
 * restent dans le namespace global et en extern "C" : dlsym cherche un nom
 * de symbole brut, il ne connait ni les namespaces ni le mangling C++. Les
 * ranger sous raylib:: ne changerait donc rien au symbole exporte - c'est
 * @addtogroup raylib les classe dans la doc, pas un namespace.
 *
 * Deux vendors exportent les memes noms : c'est voulu. Ils vivent dans deux
 * bibliotheques distinctes, et c'est ce qui permet de les interchanger sans
 * recompiler l'appelant.
 *
 * Les statiques sont locales aux fonctions : elles sont construites au
 * premier appel (magic statics C++11), donc InitAudioDevice() ne tourne que
 * si quelqu'un demande vraiment l'audio - pas parce que la dll a ete
 * chargee.
 */

/**
 * @brief Le module graphique 2D de ce vendor.
 *
 * Symbole recherche : `getGraphic2Module`. Sa presence dit au chargeur que
 * ce vendor sait dessiner en 2D.
 */
extern "C" IGraphic2Module *getGraphic2Module() {
    static RayGraphicModule instance;
    return &instance;
}

/**
 * @brief Le module graphique 3D de ce vendor.
 *
 * Symbole recherche : `getGraphic3Module`. Rend la MEME instance que
 * getGraphic2Module(), vue par l'autre bout de la chaine d'heritage :
 * RayGraphicModule implemente IGraphic3Module, qui EST un IGraphic2Module.
 * Une seule implementation, aucun risque que les deux vues divergent.
 *
 * Un vendor sans 3D n'exporte tout simplement pas ce symbole.
 */
extern "C" IGraphic3Module *getGraphic3Module() {
    static RayGraphicModule &instance = *static_cast<RayGraphicModule *>(getGraphic2Module());
    return &instance;
}

/**
 * @brief Le module audio de ce vendor.
 *
 * Symbole recherche : `getAudioModule`. Instance separee du graphique :
 * l'audio n'a aucun couplage a la fenetre, et son peripherique ne s'ouvre
 * qu'au premier appel.
 */
extern "C" IAudioModule *getAudioModule() {
    static RayAudioModule instance;
    return &instance;
}

/**
 * @brief Tout ce que cette dll fournit, sans avoir a nommer un contrat.
 *
 * Le point d'entree d'AnonymousModuleManager : un nom unique, le meme pour
 * toutes les dll, donc trouvable sans connaitre les contrats a l'avance. Le
 * chargeur trie ensuite par IModule::type().
 *
 * IModule ** termine par nullptr, et pas un std::vector : un vecteur ne
 * traverse pas un dlopen sans supposer la meme ABI de bibliotheque standard
 * des deux cotes. La conversion vers IModule * se fait ICI, ou le type
 * complet est connu, donc le decalage eventuel est applique par le
 * compilateur et pas devine par l'appelant.
 *
 * getGraphic3Module() n'y figure pas : c'est la MEME instance que la 2D,
 * vue par l'autre bout de la chaine d'heritage. L'inscrire deux fois
 * mettrait deux cases de la table sur un seul objet, et son type() ne peut
 * en declarer qu'une.
 */
extern "C" IModule **getModules() {
    static IModule *list[] = {
        getGraphic2Module(),
        getAudioModule(),
        nullptr
    };

    return list;
}

#endif /* !RAYLIB_CPP_ */

/** @} */
