#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <iostream>

std::string get_window_title(Display *display, Window window) {
    Atom prop = XInternAtom(display, "_NET_WM_NAME", False); // Modern property for window title
    Atom actual_type;
    int actual_format;
    unsigned long nitems, bytes_after;
    unsigned char *prop_return = nullptr;

    // Get the property value
    if (XGetWindowProperty(display, window, prop, 0, (~0L), False, AnyPropertyType,
                           &actual_type, &actual_format, &nitems, &bytes_after, &prop_return) == Success) {
        if (prop_return) {
            std::string title(reinterpret_cast<char*>(prop_return));
            XFree(prop_return);
            return title;
        }
    }

    return "";
}

int main() {
    Display *display = XOpenDisplay(nullptr);
    if (display == nullptr) {
        std::cerr << "Unable to open X display." << std::endl;
        return 1;
    }

    Window root = DefaultRootWindow(display);
    // Use your logic to get the window (e.g., Firefox) here
    Window targetWindow = /* Your logic to find the window */;

    std::string title = get_window_title(display, targetWindow);
    std::cout << "Window Title: " << title << std::endl;

    XCloseDisplay(display);
    return 0;
}
