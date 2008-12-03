#include <vector>

#include <boost/utility.hpp>

#include <gai/gai.h>



namespace wm_vvo {

    class LineGroup;

    class Dockapp : private boost::noncopyable { 

        const std::vector<LineGroup>& groups;
        GaiApplet applet;
        int height;

        gboolean updateGui();
        GdkPixbuf* bg;

        bool first;

        class DrawString {
            std::string text;
            int posx;
            int posy;
            int maxwidth;
            int fontsize; 
            const std::string& font;
            int red;
            int green;
            int blue;
            int feature;

            bool roate;
            int roate_state;

            GdkPixbuf* pixmap;
            int pixmap_length;
            int pixmap_height;

            void calcRoate();

            public:

            DrawString(int posx, int posy, int maxwidth, const std::string& text, int fontsize, const std::string& font, int r, int g, int b, int feature);
            ~DrawString();

            void updatePosY(int posy);
            void draw();
            void setText(const std::string& s);

        };

        std::vector<DrawString *> linegroup_strings;
        std::vector<DrawString *> station_strings;
        std::vector<DrawString *> line_strings;

        public: 
        Dockapp(const std::vector<LineGroup>& g, int *argc, char ***argv);
        ~Dockapp();

        friend gboolean updateHelper(gpointer data);
    };
    gboolean updateHelper(gpointer data);
}
