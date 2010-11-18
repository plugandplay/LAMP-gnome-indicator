#include <stdlib.h>
#include <gtk/gtk.h>
#include <sys/wait.h>

int check_apache_state();
int check_mysql_state();

void on_localhost_button_hover(GtkWidget *pButton, gpointer iContextId);
void on_localhost_button_clicked(GtkWidget *pButton, gpointer iContextId);

void on_apache_button_hover(GtkWidget *pButton, gpointer iContextId);
int on_apache_button_clicked(GtkWidget *pButton, gpointer iContextId);

void on_mysql_button_hover(GtkWidget *pButton, gpointer iContextId);
int on_mysql_button_clicked(GtkWidget *pButton, gpointer iContextId);

void clear_status_bar(GtkWidget *pButton, gpointer iContextId);

void refresh_apache_lbl();
void refresh_mysql_lbl();
