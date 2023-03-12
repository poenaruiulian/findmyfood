#include<gtk/gtk.h>
#include <string.h>
#include<time.h>

struct restaurantsData{
    GtkApplication *application;
    char title[100];
};
struct orderData{
    GtkApplication *application;
    char restaurantTitle[100],pizzaOfOrder[100],addressOfCustomer[100];
};
struct specialOrderData{
    GtkApplication *application;
    char restaurantTitle[100];
};
struct sentOrderData{
    GtkEntry* entry;
    char restaurantTitle[100],pizzaOfOrder[100],addressOfCustomer[100];
};
struct sentSpecialOrderData{
    GtkEntry *ingredients, *dimension, *address;
    char restaurantTitle[100];
};
struct prevOrderData{
    GtkApplication *application;
    int orderNumber;
};

static void activate(GtkApplication * app,gpointer user_data);
static void restaurantsScreen(GtkWidget *widget,GtkApplication *app);
static void previousNotesScreen(GtkWidget *widget,struct prevOrderData *data);
static void homeScreen(GtkWidget *widget,GtkApplication *app);
static void oneRestaurant(GtkWidget *widget,struct restaurantsData * data);
static void orderPlacementScreen(GtkWidget * widget, struct orderData * data);
static void specialOrderPlacementScreen(GtkWidget * widget, struct specialOrderData * data);

static void sendOrderFunction(GtkWidget * widget,struct sentOrderData *data){

    FILE *fptr;

    fptr = fopen("nr_comenzi.txt","r");
    int nrComanda;
    fscanf(fptr,"%d", &nrComanda);
    fclose(fptr);

    char newFileTitle[100];
    char fileNumber[3];
    sprintf(fileNumber,"%d",nrComanda);
    strcpy(newFileTitle,"comenzi/comanda");
    strcat(newFileTitle,fileNumber);
    strcat(newFileTitle,".txt");
    printf("%s",newFileTitle);

    fptr = fopen("nr_comenzi.txt","w");
    nrComanda += 1;
    fprintf(fptr,"%d", nrComanda);
    fclose(fptr);

    time_t t;
    time(&t);

    strcpy(data->addressOfCustomer, gtk_entry_get_text(data->entry));
    printf("\n");
    printf("<== COMANDA TRIMISA DE USER ESTE: ==>\n");
    printf("RESTAURANTUL SELECTAT: %s\n",data->restaurantTitle);
    printf("PIZZA SELECTATA: %s\n",data->pizzaOfOrder);
    printf("ADRESA USER: %s\n",data->addressOfCustomer);
    printf("%s\n",ctime(&t));
    printf("\n");

    fptr = fopen(newFileTitle,"w");
    fprintf(fptr,"%s\n%s\n%s\n%s\n",data->restaurantTitle,data->pizzaOfOrder,data->addressOfCustomer,ctime(&t));
    fclose(fptr);
}
static void sendSpecialOrderFunction(GtkWidget * widget,struct sentSpecialOrderData *data){

    FILE *fptr;

    fptr = fopen("nr_comenzi.txt","r");
    int nrComanda;
    fscanf(fptr,"%d", &nrComanda);
    fclose(fptr);

    char newFileTitle[100];
    char fileNumber[3];
    sprintf(fileNumber,"%d",nrComanda);
    strcpy(newFileTitle,"comenzi/comanda");
    strcat(newFileTitle,fileNumber);
    strcat(newFileTitle,".txt");
    printf("%s",newFileTitle);

    fptr = fopen("nr_comenzi.txt","w");
    nrComanda += 1;
    fprintf(fptr,"%d", nrComanda);
    fclose(fptr);

    time_t t;
    time(&t);

    char addressOfOrder[100], ingredientsOfOrder[100], dimensionOfOrder[100];

    strcpy(addressOfOrder, gtk_entry_get_text(data->address));
    strcpy(ingredientsOfOrder, gtk_entry_get_text(data->ingredients));
    strcpy(dimensionOfOrder, gtk_entry_get_text(data->dimension));

    char pizza[250];
    strcpy(pizza, "Pizza custom");

    printf("\n");
    printf("<== COMANDA TRIMISA DE USER ESTE: ==>\n");
    printf("RESTAURANTUL SELECTAT: %s\n",data->restaurantTitle);
    printf("PIZZA SELECTATA: %s\n",pizza);
    printf("INGREDIENTE SELECTATE: %s\n",ingredientsOfOrder);
    printf("DIMENSIUNE SELECTATA: %s\n",dimensionOfOrder);
    printf("ADRESA USER: %s\n",addressOfOrder);
    printf("%s\n",ctime(&t));
    printf("\n");

    fptr = fopen(newFileTitle,"a");
    fprintf(fptr,"%s\n%s\n%s\n%s\n",
            data->restaurantTitle,
            pizza,addressOfOrder,
            ctime(&t));
    fprintf(fptr,"%s\n%s\n",ingredientsOfOrder,dimensionOfOrder);
    fclose(fptr);


}
static void exitApp(){
    printf("Userul a iesit din aplicatie.");
}

///functia main care imi porneste aplicatia
int main(int argc, char **argv){
    GtkApplication * app; int status;

    app = gtk_application_new("test.one", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app,"activate",G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app),argc,argv);

    g_object_unref(app);
    return status;

}

///functia activate care initializeaza aplicatia -- primul home screen
static void activate(GtkApplication * app,gpointer user_data){

    g_print("<== Aplicatia a fost pornita cu succes ==>\n");

    GtkWidget * home;///reprezinta ecranul nostru principal

    GtkWidget * grid;

    GtkWidget * selectRestaurant;///butonul pentru selectarea restaurantului
    GtkWidget * previousNotes;///butonul pentru vizionarea istoricului
    GtkWidget * quit;

    GtkWidget * homeTitle;

    ///creem fereastra aplicatiei
    home = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(home),"FindMyFood - Acasa");
    gtk_window_set_default_size(GTK_WINDOW(home),600,400);
    gtk_window_set_position(GTK_WINDOW(home), GTK_WIN_POS_CENTER);

    homeTitle = gtk_label_new("BINE AI VENIT!!");

    selectRestaurant = gtk_button_new_with_label("Selecteaza restaurant");
    g_signal_connect(selectRestaurant,"clicked",G_CALLBACK(restaurantsScreen),app);
    g_signal_connect_swapped (selectRestaurant, "clicked", G_CALLBACK (gtk_widget_destroy),home);

    FILE *fptr;

    fptr = fopen("nr_comenzi.txt","r");
    int nrComanda;
    fscanf(fptr,"%d", &nrComanda);
    fclose(fptr);

    struct prevOrderData * data = malloc(sizeof(*data));
    data->orderNumber=1;
    data->application=app;

    previousNotes = gtk_button_new_with_label("Istoricul comenzilor");
    if(nrComanda != 1){
        g_signal_connect(previousNotes,"clicked",G_CALLBACK(previousNotesScreen),data);
        g_signal_connect_swapped(previousNotes, "clicked", G_CALLBACK (gtk_widget_destroy),home);
    }

    quit = gtk_button_new_with_label("Iesi din aplicatie");
    g_signal_connect(quit,"clicked",G_CALLBACK(exitApp),NULL);
    g_signal_connect_swapped(quit,"clicked",G_CALLBACK(gtk_widget_destroy),home);


    grid = gtk_grid_new ();
    gtk_container_add(GTK_CONTAINER(home), grid);
    gtk_grid_set_column_spacing(GTK_GRID(grid),10);
    gtk_grid_set_row_spacing(GTK_GRID(grid),10);
    gtk_grid_set_column_homogeneous ( GTK_GRID ( grid), TRUE);


    gtk_grid_attach (GTK_GRID(grid), homeTitle, 0, 0, 2, 1);
    gtk_grid_attach (GTK_GRID (grid), selectRestaurant, 0, 1, 1,1);
    gtk_grid_attach (GTK_GRID (grid), previousNotes, 1, 1, 1, 1);
    gtk_grid_attach (GTK_GRID (grid), quit, 0, 3, 2, 1);


    gtk_widget_show_all(home);

}

///home screen-ul folosit la navigari
static void homeScreen(GtkWidget *widget,GtkApplication *app){

    g_print("<== Userul se afla pe  |pagina principala| ==> \n");

    GtkWidget * home;///reprezinta ecranul nostru principal

    GtkWidget * grid;

    GtkWidget * selectRestaurant;///butonul pentru selectarea restaurantului
    GtkWidget * previousNotes;///butonul pentru vizionarea istoricului
    GtkWidget * quit;

    GtkWidget * homeTitle;

    ///creem fereastra aplicatiei
    home = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(home),"FindMyFood - Home");
    gtk_window_set_default_size(GTK_WINDOW(home),600,400);
    gtk_window_set_position(GTK_WINDOW(home), GTK_WIN_POS_CENTER);

    homeTitle = gtk_label_new("BINE AI VENIT!!");

    selectRestaurant = gtk_button_new_with_label("Selecteaza restaurant");
    g_signal_connect(selectRestaurant,"clicked",G_CALLBACK(restaurantsScreen),app);
    g_signal_connect_swapped (selectRestaurant, "clicked", G_CALLBACK (gtk_widget_destroy),home);

    FILE *fptr;

    fptr = fopen("nr_comenzi.txt","r");
    int nrComanda;
    fscanf(fptr,"%d", &nrComanda);
    fclose(fptr);

    struct prevOrderData * data = malloc(sizeof(*data));
    data->orderNumber=1;
    data->application=app;

    previousNotes = gtk_button_new_with_label("Istoricul comenzilor");
    if(nrComanda!=1){
        g_signal_connect(previousNotes,"clicked",G_CALLBACK(previousNotesScreen),data);
        g_signal_connect_swapped (previousNotes, "clicked", G_CALLBACK (gtk_widget_destroy),home);
    }

    quit = gtk_button_new_with_label("Iesi din aplicatie");
    g_signal_connect(quit,"clicked",G_CALLBACK(exitApp),NULL);
    g_signal_connect_swapped(quit,"clicked",G_CALLBACK(gtk_widget_destroy),home);


    grid = gtk_grid_new ();
    gtk_container_add(GTK_CONTAINER(home), grid);
    gtk_grid_set_column_spacing(GTK_GRID(grid),10);
    gtk_grid_set_row_spacing(GTK_GRID(grid),10);
    gtk_grid_set_column_homogeneous ( GTK_GRID ( grid), TRUE);


    gtk_grid_attach (GTK_GRID(grid), homeTitle, 0, 0, 2, 1);
    gtk_grid_attach (GTK_GRID (grid), selectRestaurant, 0, 1, 1, 1);
    gtk_grid_attach (GTK_GRID (grid), previousNotes, 1, 1, 1, 1);
    gtk_grid_attach (GTK_GRID (grid), quit, 0, 3, 2, 1);


    gtk_widget_show_all(home);
}

///pagina cu istoricul comenzilor
static void previousNotesScreen(GtkWidget *widget,struct prevOrderData * data){
    GtkWidget * previousNotes;

    ///creem fereastra aplicatiei
    previousNotes = gtk_application_window_new(data->application);
    gtk_window_set_title(GTK_WINDOW(previousNotes),"FindMyFood - Istoricul comezilor");
    gtk_window_set_default_size(GTK_WINDOW(previousNotes),600,400);
    gtk_window_set_position(GTK_WINDOW(previousNotes), GTK_WIN_POS_CENTER);

    GtkWidget * grid;
    grid = gtk_grid_new ();
    gtk_container_add(GTK_CONTAINER(previousNotes), grid);
    gtk_grid_set_column_spacing(GTK_GRID(grid),10);
    gtk_grid_set_row_spacing(GTK_GRID(grid),10);
    gtk_grid_set_column_homogeneous ( GTK_GRID ( grid), TRUE);

    char orderNumberChar[3];
    sprintf(orderNumberChar,"%d",data->orderNumber);

    char titleOfOrder[100];
    strcpy(titleOfOrder,"COMANDA CU NUMARUL < ");
    strcat(titleOfOrder,orderNumberChar);
    strcat(titleOfOrder," >");

    GtkWidget * titleOfScreen = gtk_label_new(titleOfOrder);

    GtkWidget * prevScreen;
    prevScreen = gtk_button_new_with_label("<-- Inapoi");
    g_signal_connect(prevScreen,"clicked",G_CALLBACK(homeScreen),data->application);
    g_signal_connect_swapped (prevScreen, "clicked", G_CALLBACK (gtk_widget_destroy),previousNotes);



    gtk_grid_attach (GTK_GRID(grid), titleOfScreen , 1,0,2,1);
    gtk_grid_attach (GTK_GRID(grid), prevScreen , 0,0,1,1);

    char newFileTitle[100];
    char fileNumber[3];
    sprintf(fileNumber,"%d",data->orderNumber);
    strcpy(newFileTitle,"comenzi/comanda");
    strcat(newFileTitle,fileNumber);
    strcat(newFileTitle,".txt");


    FILE *fptr;

    fptr = fopen(newFileTitle, "r");
    char restaurantTitle[100];
    char pizzaOrder[100];
    char address[100];
    char date[100];
    fgets(restaurantTitle, sizeof(restaurantTitle), fptr);
    fgets(pizzaOrder, sizeof(pizzaOrder), fptr);
    fgets(address, sizeof(address), fptr);
    fgets(date, sizeof(date), fptr);

    char aux[100];
    fgets(aux,sizeof(aux),fptr);

    char ingredients[100]="",dimension[100]="";
    fgets(ingredients, sizeof(ingredients), fptr);
    fgets(dimension, sizeof(dimension), fptr);

    if(strcmp(ingredients,"")!=0 && strcmp(dimension,"")!=0){
        strcpy(aux,"INGREDIENTE: ");
        strcat(aux, ingredients);
        GtkWidget * ingredientsReceip=gtk_label_new(aux);
        gtk_grid_attach (GTK_GRID(grid), ingredientsReceip , 0,3,3,1);

        strcpy(aux,"DIMENSIUNE: ");
        strcat(aux, dimension);
        GtkWidget * dimensionReceip=gtk_label_new(aux);
        gtk_grid_attach (GTK_GRID(grid), dimensionReceip , 0,4,3,1);
    }

    strcpy(aux,"NUMELE RESTAURANTULUI: ");
    strcat(aux, restaurantTitle);
    GtkWidget * restaurantTitleLabel=gtk_label_new(aux);
    gtk_grid_attach (GTK_GRID(grid), restaurantTitleLabel , 0,1,3,1);

    strcpy(aux,"PIZZA COMANDATA: ");
    strcat(aux, pizzaOrder);
    GtkWidget * pizzaOrderLabel=gtk_label_new(aux);
    gtk_grid_attach (GTK_GRID(grid), pizzaOrderLabel , 0,2,3,1);

    strcpy(aux,"ADRESA: ");
    strcat(aux, address);
    GtkWidget * addressLabel=gtk_label_new(aux);
    gtk_grid_attach (GTK_GRID(grid), addressLabel , 0,5,3,1);

    strcpy(aux,"DATA SI ORA: ");
    strcat(aux, date);
    GtkWidget * dateLabel=gtk_label_new(aux);
    gtk_grid_attach (GTK_GRID(grid), dateLabel , 0,6,3,1);




    printf("\n");
    printf("<== COMANDA CU NUMARUL %d ==>\n",data->orderNumber);
    printf("RESTAURANTUL SELECTAT: %s",restaurantTitle);
    printf("PIZZA SELECTATA: %s",pizzaOrder);
    printf("ADRESA USER: %s",address);
    printf("%s\n",date);
    printf("\n");
    fclose(fptr);



    fptr = fopen("nr_comenzi.txt", "r");
    int nrComenzi;
    fscanf(fptr,"%d",&nrComenzi);
    fclose(fptr);

    data->orderNumber =(data->orderNumber)+1;

    GtkWidget * nextOrder;
    nextOrder = gtk_button_new_with_label("NU EXISTA COMANDA URMATOARE");
    GtkWidget * prevOrder;
    prevOrder = gtk_button_new_with_label("NU EXISTA COMANDA ANTERIOARA");

    if(nrComenzi>(data->orderNumber)){

        nextOrder = gtk_button_new_with_label("COMANDA URMATOARE -->");
        g_signal_connect(nextOrder,"clicked",G_CALLBACK(previousNotesScreen),data);
        g_signal_connect_swapped (nextOrder, "clicked", G_CALLBACK (gtk_widget_destroy),previousNotes);

    }
    if(data->orderNumber>2){
        prevOrder = gtk_button_new_with_label("<-- COMANDA ANTERIOARA");
        struct prevOrderData * prevData = malloc(sizeof(*prevData));
        prevData->orderNumber =(data->orderNumber)-2;
        prevData->application = data->application;
        g_signal_connect(prevOrder,"clicked",G_CALLBACK(previousNotesScreen),prevData);
        g_signal_connect_swapped (prevOrder, "clicked", G_CALLBACK (gtk_widget_destroy),previousNotes);

    }
    gtk_grid_attach (GTK_GRID(grid), nextOrder , 0,7,3,1);
    gtk_grid_attach (GTK_GRID(grid), prevOrder , 0,8,3,1);

    gtk_widget_show_all(previousNotes);
}

///pagina de selectie restaurante
static void restaurantsScreen(GtkWidget *widget,GtkApplication *app){
    g_print("<== Userul se afla pe  |pagina cu restaurante| ==> \n");

    GtkWidget * restaurantSelection;

    GtkWidget * grid;

    GtkWidget * restTitle;
    restTitle = gtk_label_new("RESTAURANTE DISPONIBILE:");

    GtkWidget * prevScreen;

    struct restaurantsData * data1 = malloc(sizeof(*data1));
    struct restaurantsData * data2 = malloc(sizeof(*data2));
    struct restaurantsData * data3 = malloc(sizeof(*data3));
    data1 -> application = app;
    data2 -> application = app;
    data3 -> application = app;


    ///creem fereastra aplicatiei
    restaurantSelection = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(restaurantSelection),"FindMyFood - Selecteaza restaurantul");
    gtk_window_set_default_size(GTK_WINDOW(restaurantSelection),600,400);
    gtk_window_set_position(GTK_WINDOW(restaurantSelection), GTK_WIN_POS_CENTER);

    grid = gtk_grid_new ();
    gtk_container_add(GTK_CONTAINER(restaurantSelection), grid);
    gtk_grid_set_column_spacing(GTK_GRID(grid),10);
    gtk_grid_set_row_spacing(GTK_GRID(grid),10);
    gtk_grid_set_column_homogeneous ( GTK_GRID ( grid), TRUE);


    prevScreen = gtk_button_new_with_label("<-- Inapoi");
    g_signal_connect(prevScreen,"clicked",G_CALLBACK(homeScreen),app);
    g_signal_connect_swapped (prevScreen, "clicked", G_CALLBACK (gtk_widget_destroy),restaurantSelection);

    GtkWidget * restaurant1;///Giovanna
    GtkWidget * restaurant2;///Napoleon
    GtkWidget * restaurant3;///Thalia

    restaurant1 = gtk_button_new_with_label("Giovanna");
    strcpy(data1->title,"Giovanna");
    g_signal_connect(restaurant1,"clicked",G_CALLBACK(oneRestaurant),data1);
    g_signal_connect_swapped (restaurant1, "clicked", G_CALLBACK (gtk_widget_destroy),restaurantSelection);

    restaurant2 = gtk_button_new_with_label("Napoleon");
    strcpy(data2->title,"Napoleon");
    g_signal_connect(restaurant2,"clicked",G_CALLBACK(oneRestaurant),data2);
    g_signal_connect_swapped (restaurant2, "clicked", G_CALLBACK (gtk_widget_destroy),restaurantSelection);

    restaurant3 = gtk_button_new_with_label("Thalia");
    strcpy(data3->title,"Thalia");
    g_signal_connect(restaurant3,"clicked",G_CALLBACK(oneRestaurant),data3);
    g_signal_connect_swapped (restaurant3, "clicked", G_CALLBACK (gtk_widget_destroy),restaurantSelection);


    gtk_grid_attach (GTK_GRID(grid), restTitle, 1,0,2,1);
    gtk_grid_attach (GTK_GRID(grid), prevScreen,0,0,1,1);
    gtk_grid_attach (GTK_GRID(grid), restaurant1 , 0,1,1,1);
    gtk_grid_attach (GTK_GRID(grid), restaurant2 , 1,1,1,1);
    gtk_grid_attach (GTK_GRID(grid), restaurant3 , 2,1,1,1);


    gtk_widget_show_all(restaurantSelection);
}

///pagina unde apare meniul restaurantulului
static void oneRestaurant(GtkWidget *widget, struct restaurantsData *data){

    g_print("<== Userul se afla pe  |pagina cu %s| ==> \n", data->title);
    GtkWidget * oneRestaurant;
    GtkWidget * item1;
    GtkWidget * item2;
    GtkWidget * item3;
    GtkWidget * item4;
    GtkWidget * item5;
    GtkWidget * item6;
    GtkWidget * custom;

    GtkWidget * grid;

    GtkWidget * restTitle;

    char rTitle[100];
    strcpy(rTitle,"FindMyFood -  ");
    strcat(rTitle, data->title);

    char restaurantPizza[100];
    strcpy(restaurantPizza,"Pizza ");
    strcat(restaurantPizza,data->title);

    restTitle = gtk_label_new("SELECTEAZA O OPTIUNE DIN MENIU:");

    GtkWidget * prevScreen;

    struct orderData * pizza1 = malloc(sizeof(*pizza1));
    struct orderData * pizza2 = malloc(sizeof(*pizza2));
    struct orderData * pizza3 = malloc(sizeof(*pizza3));
    struct orderData * pizza4 = malloc(sizeof(*pizza4));
    struct orderData * pizza5 = malloc(sizeof(*pizza5));
    struct orderData * pizza6 = malloc(sizeof(*pizza6));

    struct specialOrderData * specialPizza = malloc(sizeof(*specialPizza));

    pizza1->application = data->application;
    pizza2->application = data->application;
    pizza3->application = data->application;
    pizza4->application = data->application;
    pizza5->application = data->application;
    pizza6->application = data->application;
    specialPizza->application = data->application;

    strcpy(pizza1->restaurantTitle,data->title);
    strcpy(pizza1->addressOfCustomer,"default");

    strcpy(pizza2->restaurantTitle,data->title);
    strcpy(pizza2->addressOfCustomer,"default");

    strcpy(pizza3->restaurantTitle,data->title);
    strcpy(pizza3->addressOfCustomer,"default");

    strcpy(pizza4->restaurantTitle,data->title);
    strcpy(pizza4->addressOfCustomer,"default");

    strcpy(pizza5->restaurantTitle,data->title);
    strcpy(pizza5->addressOfCustomer,"default");

    strcpy(pizza6->restaurantTitle,data->title);
    strcpy(pizza6->addressOfCustomer,"default");

    strcpy(specialPizza->restaurantTitle,data->title);



    oneRestaurant = gtk_application_window_new(data->application);
    gtk_window_set_title(GTK_WINDOW(oneRestaurant),rTitle);
    gtk_window_set_default_size(GTK_WINDOW(oneRestaurant),600,400);
    gtk_window_set_position(GTK_WINDOW(oneRestaurant), GTK_WIN_POS_CENTER);

    grid = gtk_grid_new ();
    gtk_container_add(GTK_CONTAINER(oneRestaurant), grid);
    gtk_grid_set_column_spacing(GTK_GRID(grid),10);
    gtk_grid_set_row_spacing(GTK_GRID(grid),10);
    gtk_grid_set_column_homogeneous ( GTK_GRID ( grid), TRUE);

    prevScreen = gtk_button_new_with_label("<-- Inapoi");
    g_signal_connect(prevScreen,"clicked",G_CALLBACK(restaurantsScreen),data->application);
    g_signal_connect_swapped (prevScreen, "clicked", G_CALLBACK (gtk_widget_destroy),oneRestaurant);

    item1 = gtk_button_new_with_label("Pizza Margherita");
    strcpy(pizza1->pizzaOfOrder,"Pizza Margherita");
    g_signal_connect(item1,"clicked",G_CALLBACK(orderPlacementScreen),pizza1);
    g_signal_connect_swapped(item1,"clicked",G_CALLBACK(gtk_widget_destroy),oneRestaurant);

    item2 = gtk_button_new_with_label("Pizza Prosciutto");
    strcpy(pizza2->pizzaOfOrder,"Pizza Prosciutto");
    g_signal_connect(item2,"clicked",G_CALLBACK(orderPlacementScreen),pizza2);
    g_signal_connect_swapped(item2,"clicked",G_CALLBACK(gtk_widget_destroy),oneRestaurant);

    item3 = gtk_button_new_with_label("Pizza Diavola");
    strcpy(pizza3->pizzaOfOrder,"Pizza Diavola");
    g_signal_connect(item3,"clicked",G_CALLBACK(orderPlacementScreen),pizza3);
    g_signal_connect_swapped(item3,"clicked",G_CALLBACK(gtk_widget_destroy),oneRestaurant);

    item4 = gtk_button_new_with_label(restaurantPizza);
    strcpy(pizza4->pizzaOfOrder,restaurantPizza);
    g_signal_connect(item4,"clicked",G_CALLBACK(orderPlacementScreen),pizza4);
    g_signal_connect_swapped(item4,"clicked",G_CALLBACK(gtk_widget_destroy),oneRestaurant);

    item5 = gtk_button_new_with_label("Pizza Funghi");
    strcpy(pizza5->pizzaOfOrder,"Pizza Funghi");
    g_signal_connect(item5,"clicked",G_CALLBACK(orderPlacementScreen),pizza5);
    g_signal_connect_swapped(item5,"clicked",G_CALLBACK(gtk_widget_destroy),oneRestaurant);

    item6 = gtk_button_new_with_label("Pizza Vegetariana");
    strcpy(pizza6->pizzaOfOrder,"Pizza Vegetariana");
    g_signal_connect(item6,"clicked",G_CALLBACK(orderPlacementScreen),pizza6);
    g_signal_connect_swapped(item6,"clicked",G_CALLBACK(gtk_widget_destroy),oneRestaurant);

    custom = gtk_button_new_with_label("Creeaza-ti propria pizza!!");
    g_signal_connect(custom,"clicked",G_CALLBACK(specialOrderPlacementScreen),specialPizza);
    g_signal_connect_swapped(custom,"clicked",G_CALLBACK(gtk_widget_destroy),oneRestaurant);


    gtk_grid_attach (GTK_GRID(grid), restTitle, 1,0,2,1);
    gtk_grid_attach (GTK_GRID(grid), prevScreen,0,0,1,1);

    gtk_grid_attach (GTK_GRID(grid), item1,0,1,1,1);
    gtk_grid_attach (GTK_GRID(grid), item2,1,1,1,1);
    gtk_grid_attach (GTK_GRID(grid), item3,2,1,1,1);

    gtk_grid_attach (GTK_GRID(grid), item4,0,2,1,1);
    gtk_grid_attach (GTK_GRID(grid), item5,1,2,1,1);
    gtk_grid_attach (GTK_GRID(grid), item6,2,2,1,1);

    gtk_grid_attach (GTK_GRID(grid), custom,0,3,3,1);


    gtk_widget_show_all(oneRestaurant);

}

///pagina unde se plaseaza comanda
static void orderPlacementScreen(GtkWidget * widget, struct orderData *data){
    g_print("<== Utilizatorul doreste sa plaseze o comanda ==>\n");

    GtkWidget * orderPlacement;

    GtkWidget * pizzaOrder;
    GtkWidget * resturantOrder;

    GtkWidget * prevScreen;



    char pizzaOrderLabel[100];
    strcpy(pizzaOrderLabel,"PIZZA SELECTATA: ");
    strcat(pizzaOrderLabel,data->pizzaOfOrder);
    pizzaOrder = gtk_label_new(pizzaOrderLabel);

    char resturantOrderLabel[100];
    strcpy(resturantOrderLabel,"RESTAURANTUL SELECTAT: ");
    strcat(resturantOrderLabel,data->restaurantTitle);
    resturantOrder = gtk_label_new(resturantOrderLabel);

    GtkWidget * screenTitle = gtk_label_new("DETALIILE COMENZII:");
    GtkWidget * labelAdress = gtk_label_new("INTRODU ADRESA:");

    GtkWidget * grid;

    orderPlacement = gtk_application_window_new(data->application);
    gtk_window_set_title(GTK_WINDOW(orderPlacement),"FindMyFood - Finalizeaza comanda");
    gtk_window_set_default_size(GTK_WINDOW(orderPlacement),600,400);
    gtk_window_set_position(GTK_WINDOW(orderPlacement), GTK_WIN_POS_CENTER);

    grid = gtk_grid_new ();
    gtk_container_add(GTK_CONTAINER(orderPlacement), grid);
    gtk_grid_set_column_spacing(GTK_GRID(grid),10);
    gtk_grid_set_row_spacing(GTK_GRID(grid),10);
    gtk_grid_set_column_homogeneous ( GTK_GRID ( grid), TRUE);

    GtkWidget *adressEntry;
    adressEntry = gtk_entry_new();


    GtkWidget * sendOrder;
    sendOrder = gtk_button_new_with_label("Trimite comanda!!");

    struct sentOrderData *dataToSend = malloc(sizeof(*dataToSend));
    strcpy(dataToSend->pizzaOfOrder,data->pizzaOfOrder);
    strcpy(dataToSend->restaurantTitle,data->restaurantTitle);
    dataToSend->entry = adressEntry;

    g_signal_connect(sendOrder,"clicked",G_CALLBACK(sendOrderFunction),dataToSend);

    struct restaurantsData *oneRestData = malloc(sizeof(*oneRestData));
    oneRestData->application = data->application;
    strcpy(oneRestData->title,data->restaurantTitle);
    prevScreen = gtk_button_new_with_label("<-- Inapoi");
    g_signal_connect(prevScreen,"clicked",G_CALLBACK(oneRestaurant),oneRestData);
    g_signal_connect_swapped (prevScreen, "clicked", G_CALLBACK (gtk_widget_destroy),orderPlacement);

    gtk_grid_attach (GTK_GRID(grid), screenTitle,1,0,2,1);
    gtk_grid_attach (GTK_GRID(grid), prevScreen,0,0,1,1);
    gtk_grid_attach (GTK_GRID(grid), resturantOrder, 0,1,3,1);
    gtk_grid_attach (GTK_GRID(grid), pizzaOrder, 0,2,3,1);
    gtk_grid_attach (GTK_GRID(grid), labelAdress, 0,3,3,1);
    gtk_grid_attach (GTK_GRID(grid), adressEntry, 0,4,3,1);
    gtk_grid_attach (GTK_GRID(grid), sendOrder, 0,5,3,1);




    gtk_widget_show_all(orderPlacement);

}

///pagina pentru creearea propiei pizza
static void specialOrderPlacementScreen(GtkWidget * widget, struct specialOrderData * data){
    g_print("<== Utilizatorul doreste sa isi creeze propria pizza ==>\n");

    GtkWidget * specialOrderPlacement;

    GtkWidget * screenTitle = gtk_label_new("CREEAZA PROPRIA PIZZA SI TRIMITE COMANDA:");

    GtkWidget * resturantOrder;
    char resturantOrderLabel[100];
    strcpy(resturantOrderLabel,"RESTAURANTUL SELECTAT:  ");
    strcat(resturantOrderLabel,data->restaurantTitle);
    resturantOrder = gtk_label_new(resturantOrderLabel);

    GtkWidget * grid;
    GtkWidget * prevScreen;

    struct restaurantsData * restData = malloc(sizeof(*restData));
    restData ->application = data->application;
    strcpy(restData->title,data->restaurantTitle);

    specialOrderPlacement = gtk_application_window_new(data->application);
    gtk_window_set_title(GTK_WINDOW(specialOrderPlacement),"FindMyFood - Creeaza-ti propria pizza");
    gtk_window_set_default_size(GTK_WINDOW(specialOrderPlacement),600,400);
    gtk_window_set_position(GTK_WINDOW(specialOrderPlacement), GTK_WIN_POS_CENTER);

    grid = gtk_grid_new ();
    gtk_container_add(GTK_CONTAINER(specialOrderPlacement), grid);
    gtk_grid_set_column_spacing(GTK_GRID(grid),10);
    gtk_grid_set_row_spacing(GTK_GRID(grid),10);
    gtk_grid_set_column_homogeneous ( GTK_GRID ( grid), TRUE);

    prevScreen = gtk_button_new_with_label("<-- Inapoi");
    g_signal_connect(prevScreen,"clicked",G_CALLBACK(oneRestaurant),restData);
    g_signal_connect_swapped (prevScreen, "clicked", G_CALLBACK (gtk_widget_destroy),specialOrderPlacement);

    GtkWidget *adressEntry;
    GtkWidget *adressLabel = gtk_label_new("INTRODU ADRESA:");
    GtkWidget *ingredientsEntry;
    GtkWidget *ingredientsLabel = gtk_label_new("INTRODU INGREDIENTELE DORITE:");
    GtkWidget *dimensionEntry;
    GtkWidget *dimensionLabel = gtk_label_new("INTRODU DIMENSIUNEA DORITA mica/medie/mare:");

    adressEntry = gtk_entry_new();
    ingredientsEntry = gtk_entry_new();
    dimensionEntry = gtk_entry_new();

    gtk_grid_attach (GTK_GRID(grid), screenTitle, 1,0,2,1);
    gtk_grid_attach (GTK_GRID(grid), prevScreen, 0,0,1,1);
    gtk_grid_attach (GTK_GRID(grid), resturantOrder, 0,1,3,1);
    gtk_grid_attach (GTK_GRID(grid), ingredientsLabel, 0,2,3,1);
    gtk_grid_attach (GTK_GRID(grid), ingredientsEntry, 0,3,3,1);
    gtk_grid_attach (GTK_GRID(grid), dimensionLabel, 0,4,3,1);
    gtk_grid_attach (GTK_GRID(grid), dimensionEntry, 0,5,3,1);
    gtk_grid_attach (GTK_GRID(grid), adressLabel, 0,6,3,1);
    gtk_grid_attach (GTK_GRID(grid), adressEntry, 0,7,3,1);

    struct sentSpecialOrderData * specOrder = malloc(sizeof(*specOrder));
    specOrder->address = adressEntry;
    specOrder->dimension=dimensionEntry;
    specOrder->ingredients=ingredientsEntry;
    strcpy(specOrder->restaurantTitle,data->restaurantTitle);

    GtkWidget * placeOrder = gtk_button_new_with_label("Trimite comanda!!");
    g_signal_connect(placeOrder,"clicked",G_CALLBACK(sendSpecialOrderFunction),specOrder);

    gtk_grid_attach(GTK_GRID(grid), placeOrder, 0,8,3,1);

    gtk_widget_show_all(specialOrderPlacement);


}




