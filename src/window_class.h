#ifndef WINDOWCLASS_H
#define WINDOWCLASS_H

    #include <X11/Xlib.h>
    #include <memory>
    #include <string>
    #include <vector>

    //crea una ventana de X11
    class DisplayClass;

    class WindowClass{
        private:
            Window window;
            const std::shared_ptr<DisplayClass> displayClass;
            std::unique_ptr<std::vector<Atom> > atoms;
            //logica
        public:
            //constructor
            WindowClass(
                    const std::shared_ptr<DisplayClass> &displayClass,
                    const int x,
                    const int y,
                    const unsigned int width,
                    const unsigned int height,
                    const unsigned int color,
                    const bool resize);
            //destructor
            ~WindowClass();
            //asigna la ventana al contexto
            void map();
            //agrega un protocolo WM
            Atom addWMProtocol(const std::string &name);
            //establece el tamaño mayor y menor de la ventana
            void setNormalSize(int minWidth,int minHeight,int maxWidth,int maxHeight);
            //establece el titulo de la ventana
            void setTitle(const std::string &title);
            //estable una mascara de eventos
            void setEventMask(int mask);
            //getter y setter
            Window get();
            const std::shared_ptr<DisplayClass> getDisplay();
    };

    #include "display_class.h"

#endif
