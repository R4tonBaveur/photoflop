#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#include "C-Utils/stack/stack.h"
#include <stdlib.h>
#include <math.h>


// GTK items
GtkWidget* Window;
GtkWidget* MenuBar;
GtkWidget* Selection;
GtkWidget* Filters;
GtkWidget* Drawing;
GtkWidget* Actions;
GtkWidget* DrawingArea;
GtkWidget* Layout; 
GtkWidget* GrayScaleButton;
GtkWidget* SepiaButton;
GtkWidget* DarknessButton;
GtkWidget* BrightnessButton;
GtkWidget* BlurButton;
GtkWidget* OilPainting;
GtkWidget* BinarizationButton;
GtkWidget* GoBackButton;
GtkWidget* SaveButton;
GtkWidget* RectangleButton;
GtkWidget* CircleButton;
GtkWidget* AllButton;
GtkWidget* ColorButton;
GtkWidget* DrawCircleButton;
GtkWidget* DrawDiamondButton;
GtkWidget* DrawHexagoneButton;
GtkWidget* DrawSquareButton;
GtkWidget* DrawStarButton;
GtkWidget* DrawTextBubbleButton;
GtkWidget* DrawThunderButton;
GtkWidget* DrawTriangleButton;
GtkWidget* DrawRectangleButton;
GtkWidget* FileName;

//Other declarations
struct SStack{
    struct SStack* old;
    SDL_Surface* surface;
};
struct SStack* s;
SDL_Surface* Surface;
SDL_Surface* SelectionZone;
char* filename;
int SurfaceVersion;

/**/
// get coordonates of a clic
void get_coord(SDL_Surface* image, size_t *x, size_t *y)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window * window = SDL_CreateWindow("Photoflop",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, image->w, image->h, 0);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer,
        image);

    int quit = 0;
    SDL_Event event;
    while (!quit)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
        case SDL_QUIT:
            quit = 1;
            break;
        case SDL_MOUSEBUTTONDOWN:
            *x = event.motion.x;
            *y = event.motion.y;
            quit = 1;
            break;
        }
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

// Resize the Surface to fit the app
SDL_Surface* resize(SDL_Surface* image,int target){
    int width = target;
    int height = target;
    if(image->w>image->h){
        height = image->h*target/image->w;
    } else {
        width = image->w*target/image->h;
    }
    SDL_Surface* res = SDL_CreateRGBSurface(0,width,height,32,0,0,0,0);
    for(int x=0;x<width;x++){
        for(int y=0;y<height;y++){
            setPixel(res,x,y,
                    getPixel(image,x*image->w/target,y*image->h/target));
        }
    }
    SDL_FreeSurface(image);
    return res;
}

// clamp an integer
int clamp(int val, int maxval){
    if(val>maxval)
        return maxval;
    return val;
}

// Returns a binarization threshold using otsu algorithm
int otsu(SDL_Surface* image){
    int precision = 255;
    int histogram[precision];
    float alpha[precision];
    int totalPixel = image->w * image->h;
    for(int i=0;i<precision;i++){
        histogram[i]=0;
        alpha[i] = 0;
    }
    for(int x=0;x<image->w;x++){
        for(int y=0;y<image->h;y++){
            Uint32 pixel = getPixel(image,x,y);
            Uint8 val;
            SDL_GetRGB(pixel, image->format, &val, &val, &val);
            if(precision*val/255<precision){
				histogram[precision*val/255]+=1;
			}
        }
    }
    for(int i=0;i<precision;i++){
        float Wb = 0;
        float Ub = 0;
        for(int backgroundLayers=0;backgroundLayers < i;backgroundLayers++){
            Wb+=histogram[backgroundLayers];
            Ub+=backgroundLayers*histogram[backgroundLayers];
        }
        Ub/=Wb;
        Wb/=totalPixel;
        float Wf = 0;
        float Uf = 0;
        for(int foregroundLayers=i;foregroundLayers<precision;foregroundLayers++){
            Wf+=histogram[foregroundLayers];
            Uf+=foregroundLayers*histogram[foregroundLayers];
        }
        Uf/=Wf;
        Wf/=totalPixel;
        alpha[i] = Wb * Wf * ((Ub-Uf) * (Ub - Uf));
    }
    int maxAlphaIndex = 1;
    for(int i=1;i<precision;i++){
        if(alpha[i]>alpha[maxAlphaIndex] || i==0){
            maxAlphaIndex = i;
        }
    }
    return maxAlphaIndex * 255 / precision;
}

// update the surface stack
void update(){
    struct SStack* new = malloc(sizeof(struct SStack));
    SDL_Surface* newSurface = SDL_CreateRGBSurface(0,Surface->w,Surface->h,32,0,0,0,0);
    for(int x=0;x<Surface->w;x++){
        for(int y=0;y<Surface->h;y++){
            setPixel(newSurface,x,y,getPixel(Surface,x,y));
        }
    }
    new->old = s;
    new->surface = newSurface;
    s = new;
    SurfaceVersion++;
    printf("Image Version: %d\n",SurfaceVersion);
}

// draw a black shape from a file
void drawShape(char* shape){
    size_t x1,y1,x2,y2;
    get_coord(Surface,&x1,&y1);
    get_coord(Surface,&x2,&y2);
    
    if(x2<x1){
        size_t mem = x1;
        x1 = x2;
        x2 = mem;
    }
    if(y2<y1){
        size_t mem = y1;
        y1 = y2;
        y2 = mem;
    }
    int min = x2-x1;
    if(y1-y2<min){
        min = y2-y1;
    }
    SDL_Surface* square = resize(loadImage(shape),min);
    for(int x=0;x<min;x++){
        for(int y=0;y<min;y++){
            if(!getPixel(square,x,y)){
                setPixel(Surface,x1+x,y1+y,0);
            }
        }
    }
    //displaySurface(square);
    gtk_widget_queue_draw(DrawingArea);
    update();
}

// Callback Functions
void on_draw(GtkWidget *widget, cairo_t *cr, gpointer data){
    if(!Surface)
        return;
    int width, height;
    Uint8 r,g,b;
    Uint32 C;
    width = Surface->w;
    height = Surface->h;
    for(int x=0;x<width;x++){
        for(int y=0;y<height;y++){
            C = getPixel(Surface, x, y);
            SDL_GetRGB(C, Surface->format,&r, &g, &b);
            cairo_set_source_rgb(cr,
                    (double) r/(double) 255,
                    (double) g/(double) 255,
                    (double) b/(double) 255);
            cairo_rectangle(cr, x, y, 1 , 1);
            cairo_fill(cr);
        }
    }
    cairo_stroke(cr);
}

void on_GoBack(){
    if(SurfaceVersion > 0){
        free(s->surface);
        s = s->old;
        Surface = s->surface;
        gtk_widget_queue_draw(DrawingArea);
        SurfaceVersion--;
        printf("Image Version: %d\n",SurfaceVersion);
    } else {
        printf("can't go back from original image\n");
    }
}

void on_GrayScale(){
    int width = Surface->w;
    int height = Surface->h;
    for(int x = 0; x<width; x++)
    {
        for(int y = 0; y<height; y++)
        {
            Uint32 pixel = getPixel(Surface, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, Surface->format, &r, &g, &b);
            Uint8 avg = 0.3*r+0.59*g+0.11*b;
            pixel = SDL_MapRGB(Surface->format, avg, avg, avg);
            if(getPixel(SelectionZone,x,y))
                setPixel(Surface, x, y, pixel);
        }
    }
    gtk_widget_queue_draw(DrawingArea);
    update();
}

void on_Sepia(){
    for(int x = 0; x < Surface->w; x++)
    {
        for(int y = 0; y < Surface->h; y++)
        {
            Uint32 pixel = getPixel(Surface, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, Surface->format, &r, &g, &b);
            pixel = SDL_MapRGB(
                Surface->format, 
                clamp(r * 0.393 + g * 0.769 + b * 0.189,255), 
                clamp(r * 0.349 + g * 0.686 + b * 0.168,255), 
                clamp(r * 0.272 + g * 0.534 + b * 0.131,255));
            if(getPixel(SelectionZone,x,y))
                setPixel(Surface, x, y, pixel);
        }
    }
    update();
    gtk_widget_queue_draw(DrawingArea);
}

void on_Darkness(){
    int n = 5;
    for(size_t x = 0; x < Surface->w; x++)
    {
        for(size_t y = 0; y < Surface->h; y++)
        {
            Uint32 pixel = getPixel(Surface, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, Surface->format, &r, &g, &b);
            pixel = SDL_MapRGB(Surface->format, (int) (255 * pow((double) r/255, n)), (int) (255 * pow((double) g/255, n)), (int) (255 * pow((double) b/255, n)));
            if(getPixel(SelectionZone,x,y))
                setPixel(Surface, x, y, pixel);
        }
    }
    update();
    gtk_widget_queue_draw(DrawingArea);
}

void on_Brightness(){
    float n = 0.5f;
    for(size_t x = 0; x < Surface->w; x++)
    {
        for(size_t y = 0; y < Surface->h; y++)
        {
            Uint32 pixel = getPixel(Surface, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, Surface->format, &r, &g, &b);
            pixel = SDL_MapRGB(Surface->format, (int) (255 * pow((double) r/255, n)), (int) (255 * pow((double) g/255, n)), (int) (255 * pow((double) b/255, n)));
            if(getPixel(SelectionZone,x,y))
                setPixel(Surface, x, y, pixel);
        }
    }
    update();
    gtk_widget_queue_draw(DrawingArea);
}

void on_Blur(){
    SDL_Surface* image = Surface;
    int precision = 2;
    Uint32 matrix[image->w][image->h];
    for(int x=0;x<image->w;x++){
        for(int y=0;y<image->h;y++){
            Uint32 totalPixels = 0;
            Uint32 RSum = 0;
            Uint32 GSum = 0;
            Uint32 BSum = 0;
            for(int xShift=-precision;xShift<precision;xShift++){
                for(int yShift=-precision;yShift<precision;yShift++){
                    if(x+xShift>=0&&x+xShift<image->w&&y+yShift>=0&&y+yShift<image->h){
                        totalPixels++;
                        uint8_t r,g,b;
                        SDL_GetRGB(getPixel(image,xShift+x,yShift+y), image->format, &r, &g, &b);
                        RSum+=r;
                        GSum+=g;
                        BSum+=b;
                    }
                }
            }
            Uint32 R = RSum/totalPixels;
            Uint32 G = GSum/totalPixels;
            Uint32 B = BSum/totalPixels;
            matrix[x][y] = RGBToUint32(image,R,G,B);
        }
    }
    for(int x=0;x<image->w;x++){
        for(int y=0;y<image->h;y++){
            if(getPixel(SelectionZone,x,y))
                setPixel(image,x,y,matrix[x][y]);
        }
    }
    update();
    gtk_widget_queue_draw(DrawingArea);
}

void on_OilPainting(){
    int r = 3;
    int intensityLevels = 20;
    int intensityCount[intensityLevels];
    int averageR[intensityLevels];
    int averageG[intensityLevels];
    int averageB[intensityLevels];
    for(int x=0;x<Surface->w;x++){
        for(int y=0;y<Surface->h;y++){
            for(int i=0;i<intensityLevels;i++){
                intensityCount[i] = 0;
                averageR[i] = 0;
                averageG[i] = 0;
                averageB[i] = 0;
            }
            for(int px=x-r;px<x+r;px++){
                for(int py=y-r;py<y+r;py++){
                    if(px>0 && px<Surface->w && py>0 && py<Surface->h && ((px-x)*(px-x)+(py-y)*(py-y))<r*r){
                        Uint32 c = getPixel(Surface,px,py);
                        Uint8 r,g,b;
                        SDL_GetRGB(c,Surface->format,&r,&g,&b);
                        int curIntensity = (int)((double)((r+g+b)/3)*intensityLevels)/255.0f;
                        intensityCount[curIntensity]++;
                        averageR[curIntensity] += r;
                        averageG[curIntensity] += g;
                        averageB[curIntensity] += b;
                    }
                }
            }
            int maxIndex = 0;
            int curMax = intensityCount[0];
            for(int i=0;i<intensityLevels;i++){
                if(curMax<intensityCount[i]){
                    curMax = intensityCount[i];
                    maxIndex = i;
                }
            }
            Uint8 r,g,b;
            r = averageR[maxIndex] / curMax;
            g = averageG[maxIndex] / curMax;
            b = averageB[maxIndex] / curMax;
            if(getPixel(SelectionZone,x,y))
                setPixel(Surface,x,y,SDL_MapRGB(Surface->format,r,g,b));
        }
    }
    update();
    gtk_widget_queue_draw(DrawingArea);
}

void on_Binarization(){
    int threshold = otsu(Surface);
    for(int x=0;x<Surface->w;x++){
        for(int y=0;y<Surface->h;y++){
            if(getPixel(SelectionZone,x,y)){
                Uint32 c = getPixel(Surface,x,y);
                Uint8 r,g,b;
                SDL_GetRGB(c,Surface->format,&r,&g,&b);
                int comp = (((r+g+b)/3)>=threshold)*255;
                setPixel(Surface,x,y,SDL_MapRGB(Surface->format,comp,comp,comp));
            }
        }
    }
    update();
    gtk_widget_queue_draw(DrawingArea);
}

void on_save(){
    char path[64];
    sprintf(path,"exports/%s",filename);
    saveImage(Surface,path);
}

void on_Rectangle(){
    size_t x1,y1,x2,y2;
    get_coord(Surface,&x1,&y1);
    get_coord(Surface,&x2,&y2);
    /*printf("%zu\n",x1);
    printf("%zu\n",y1);
    printf("%zu\n",x2);
    printf("%zu\n",y2);*/
    if(x2<x1){
        size_t mem = x1;
        x1 = x2;
        x2 = mem;
    }
    if(y2<y1){
        size_t mem = y1;
        y1 = y2;
        y2 = mem;
    }
    for(int x=0;x<SelectionZone->w;x++){
        for(int y=0;y<SelectionZone->h;y++){
            if(x>=x1 && x<=x2 && y>=y1 && y<=y2){
                setPixel(SelectionZone,x,y,RGBToUint32(SelectionZone,255,255,255));
            } else {
                setPixel(SelectionZone,x,y,0);
            }
        }
    }
    update();
    gtk_widget_queue_draw(DrawingArea);
}

void on_Circle(){
    size_t xCenter,yCenter,xRadius,yRadius;
    get_coord(Surface,&xCenter,&yCenter);
    get_coord(Surface,&xRadius,&yRadius);
    for(int x=0;x<SelectionZone->w;x++){
        for(int y=0;y<SelectionZone->h;y++){
            setPixel(SelectionZone,x,y,0);
        }
    }
    size_t radius = sqrt((xCenter-xRadius)*(xCenter-xRadius)+(yCenter-yRadius)*(yCenter-yRadius));
    for(int x=xCenter-radius;x<SelectionZone->w;x++){
        for(int y=yCenter-radius;y<SelectionZone->h;y++){
            if(x>=0 && x<xCenter+radius && y>=0 && y<yCenter+radius){
                if(sqrt((x - xCenter) * (x - xCenter) + (y - yCenter) * (y - yCenter))<=radius){
                    setPixel(SelectionZone,x,y,RGBToUint32(SelectionZone,255,255,255));
                } else {
                    setPixel(SelectionZone,x,y,0);
                }
            }
        }
    }
    //displaySurface(SelectionZone);
}

void on_All(){
    for(int x=0;x<SelectionZone->w;x++){
        for(int y=0;y<SelectionZone->h;y++){
            setPixel(SelectionZone,x,y,RGBToUint32(SelectionZone,255,255,255));
        }
    }
}

void on_Color(){
    size_t x1,y1;
    get_coord(Surface,&x1,&y1);
    Uint32 c = getPixel(Surface,x1,y1);
    for(int x=0;x<Surface->w;x++){
        for(int y=0;y<Surface->h;y++){
            if(getPixel(Surface,x,y) == c){
                setPixel(SelectionZone,x,y,RGBToUint32(SelectionZone,255,255,255));
            } else {
                setPixel(SelectionZone,x,y,0);
            }
        }
    }
}

void on_DrawCircle(){
    drawShape("drawings/circle.png");
}
void on_DrawDiamond(){
    drawShape("drawings/diamond.png");
}
void on_DrawHexagone(){
    drawShape("drawings/hexagone.png");
}
void on_DrawSquare(){
    drawShape("drawings/square.png");
}
void on_DrawStar(){
    drawShape("drawings/star.png");
}
void on_DrawTextBubble(){
    drawShape("drawings/text_bubble.png");
}
void on_DrawThunder(){
    drawShape("drawings/thunder.png");
}
void on_DrawTriangle(){
    drawShape("drawings/triangle.png");
}
void on_DrawRectangle(){
    size_t x1,y1,x2,y2;
    get_coord(Surface,&x1,&y1);
    get_coord(Surface,&x2,&y2);
    if(x2<x1){
        size_t mem = x1;
        x1 = x2;
        x2 = mem;
    }
    if(y2<y1){
        size_t mem = y1;
        y1 = y2;
        y2 = mem;
    }
    for(int x=x1;x<x2;x++){
        setPixel(Surface,x,y1,0);
        setPixel(Surface,x,y2,0);
    }
    for(int y=y1;y<y2;y++){
        setPixel(Surface,x1,y,0);
        setPixel(Surface,x2,y,0);
    }
    update();
    gtk_widget_queue_draw(DrawingArea);
}


// Main function
int main(int argc, char **argv){
    if(argc<2){
        errx(1,"Usage: ./gui <filename>");
    }
    // Initializes GTK
    gtk_init(NULL, NULL);

    // Loads builder from XML file
    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "gui.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    // The image
    filename = argv[1];
    Surface = resize(loadImage(filename),500);
    if(Surface==NULL){
        printf("Error while loading surface\n");
        return 1;
    }
    SurfaceVersion = -1;

    // The zone image
    SelectionZone = SDL_CreateRGBSurface(0,Surface->w,Surface->h,32,0,0,0,0);
    for(int x=0;x<SelectionZone->w;x++){
        for(int y=0;y<SelectionZone->h;y++){
            setPixel(SelectionZone,x,y,RGBToUint32(SelectionZone,255,255,255));
        }
    }
    
    // Surface stack
    s = malloc(sizeof(struct SStack));
    s->old = NULL;
    s->surface = Surface;
    update();

    // Getting the different components from the builder
    Window = GTK_WINDOW(gtk_builder_get_object(builder, "Window"));
    MenuBar = GTK_WIDGET(gtk_builder_get_object(builder,"MenuBar"));
    SaveButton = GTK_WIDGET(gtk_builder_get_object(builder,"SaveButton"));
    Layout = GTK_WIDGET(gtk_builder_get_object(builder,"BoxLayout"));
    Selection = GTK_WIDGET(gtk_builder_get_object(builder,"Selection"));
    Filters = GTK_WIDGET(gtk_builder_get_object(builder,"Filters"));
    Drawing = GTK_WIDGET(gtk_builder_get_object(builder,"Drawing"));
    Actions = GTK_WIDGET(gtk_builder_get_object(builder,"Actions"));
    DrawingArea = GTK_WIDGET(gtk_builder_get_object(builder,"DrawingArea"));    
    GrayScaleButton = GTK_WIDGET(gtk_builder_get_object(builder,"GrayScaleButton"));
    SepiaButton = GTK_WIDGET(gtk_builder_get_object(builder,"SepiaButton"));
    DarknessButton = GTK_WIDGET(gtk_builder_get_object(builder,"DarknessButton"));
    BrightnessButton = GTK_WIDGET(gtk_builder_get_object(builder,"BrightnessButton"));
    GoBackButton = GTK_WIDGET(gtk_builder_get_object(builder,"GoBackButton"));
    BlurButton = GTK_WIDGET(gtk_builder_get_object(builder,"BlurButton"));
    OilPainting = GTK_WIDGET(gtk_builder_get_object(builder,"OilPaintingButton"));
    RectangleButton = GTK_WIDGET(gtk_builder_get_object(builder,"RectangleButton"));
    BinarizationButton = GTK_WIDGET(gtk_builder_get_object(builder,"BinarizationButton"));
    CircleButton = GTK_WIDGET(gtk_builder_get_object(builder,"CircleButton"));
    AllButton = GTK_WIDGET(gtk_builder_get_object(builder,"AllButton"));
    ColorButton = GTK_WIDGET(gtk_builder_get_object(builder,"ColorButton"));
    FileName = GTK_WIDGET(gtk_builder_get_object(builder,"FileName"));
    DrawCircleButton = GTK_WIDGET(gtk_builder_get_object(builder,"DrawCircleButton"));
    DrawDiamondButton = GTK_WIDGET(gtk_builder_get_object(builder,"DrawDiamondButton"));
    DrawHexagoneButton = GTK_WIDGET(gtk_builder_get_object(builder,"DrawHexagoneButton"));
    DrawSquareButton = GTK_WIDGET(gtk_builder_get_object(builder,"DrawSquareButton"));
    DrawStarButton = GTK_WIDGET(gtk_builder_get_object(builder,"DrawStarButton"));
    DrawTextBubbleButton = GTK_WIDGET(gtk_builder_get_object(builder,"DrawTextBubbleButton"));
    DrawThunderButton = GTK_WIDGET(gtk_builder_get_object(builder,"DrawThunderButton"));
    DrawTriangleButton = GTK_WIDGET(gtk_builder_get_object(builder,"DrawTriangleButton"));
    DrawRectangleButton = GTK_WIDGET(gtk_builder_get_object(builder,"DrawRectangleButton"));
    // Displaying the window
    gtk_window_set_default_size(GTK_WINDOW(Window),500,500);
    gtk_window_set_resizable(Window, FALSE);    
    gtk_widget_show_all(Window);
    gtk_widget_show(Window);

    
    // Connecting the signals
    g_signal_connect(Window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(SaveButton,"activate", G_CALLBACK(on_save), NULL);
    g_signal_connect(DrawingArea,"draw", G_CALLBACK(on_draw), NULL);
    g_signal_connect(GoBackButton,"activate",G_CALLBACK(on_GoBack), NULL);
    g_signal_connect(GrayScaleButton,"activate",G_CALLBACK(on_GrayScale), NULL);
    g_signal_connect(SepiaButton,"activate",G_CALLBACK(on_Sepia), NULL);
    g_signal_connect(DarknessButton,"activate",G_CALLBACK(on_Darkness), NULL);
    g_signal_connect(BrightnessButton,"activate",G_CALLBACK(on_Brightness), NULL);
    g_signal_connect(BlurButton,"activate",G_CALLBACK(on_Blur), NULL);
    g_signal_connect(OilPainting,"activate",G_CALLBACK(on_OilPainting), NULL);
    g_signal_connect(BinarizationButton,"activate",G_CALLBACK(on_Binarization), NULL);
    g_signal_connect(RectangleButton,"activate",G_CALLBACK(on_Rectangle), NULL);
    g_signal_connect(CircleButton,"activate",G_CALLBACK(on_Circle), NULL);
    g_signal_connect(AllButton,"activate",G_CALLBACK(on_All), NULL);
    g_signal_connect(ColorButton,"activate",G_CALLBACK(on_Color), NULL);
    g_signal_connect(DrawCircleButton,"activate",G_CALLBACK(on_DrawCircle), NULL);
    g_signal_connect(DrawDiamondButton,"activate",G_CALLBACK(on_DrawDiamond), NULL);
    g_signal_connect(DrawHexagoneButton,"activate",G_CALLBACK(on_DrawHexagone), NULL);
    g_signal_connect(DrawSquareButton,"activate",G_CALLBACK(on_DrawSquare), NULL);
    g_signal_connect(DrawStarButton,"activate",G_CALLBACK(on_DrawStar), NULL);
    g_signal_connect(DrawTextBubbleButton,"activate",G_CALLBACK(on_DrawTextBubble), NULL);
    g_signal_connect(DrawThunderButton,"activate",G_CALLBACK(on_DrawThunder), NULL);
    g_signal_connect(DrawTriangleButton,"activate",G_CALLBACK(on_DrawTriangle), NULL);
    g_signal_connect(DrawRectangleButton,"activate",G_CALLBACK(on_DrawRectangle), NULL);
    gtk_label_set_text(GTK_LABEL(FileName), filename);

    // Runs the main loop.
    gtk_main();
    gtk_widget_destroy(Window);
    gtk_widget_destroy(MenuBar);
    // Exits.
    return 0;
}
