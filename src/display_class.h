#ifndef DIPLAYCLASS_H
#define DIPLAYCLASS_H

    #include <X11/Xlib.h>
    #include <memory>
    #include <string>
    #include <vector>

    //inicia un contexto de X11
    class DisplayClass{
        private:
            Display *display;
            std::unique_ptr<std::vector<Atom> > atoms;
        public:
            //constructor
            DisplayClass();
            //destructor
            ~DisplayClass();
            //recoje los eventos de X11
            void next(XEvent &event);
            //getter setter
            Display* get();
    };

#endif
