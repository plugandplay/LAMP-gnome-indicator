#include "lamp-indicator.h"

#ifndef DEBUG
//#define DEBUG 0
#endif

static GtkWidget *status_bar;
static int apache_status, mysql_status;
static GtkWidget* apache_lbl = NULL, *mysql_lbl = NULL;
static GtkWidget *pVBox, *apacheHBox, *mysqlHBox;

int main(int argc, char **argv)
{
	GtkWidget* main_window = NULL;
	GtkWidget* title_lbl = NULL;
	GtkWidget* apache_button, *mysql_button, *localhost_button;
	gchar* tmp_txt = NULL;
	gint move_x = 200, move_y = 200;

	gtk_init(&argc, &argv);

	// Main window initialization.
	main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(main_window), "LAMP");
	gtk_window_set_default_size(GTK_WINDOW(main_window), 320, 100);
	gtk_window_move(GTK_WINDOW(main_window), move_x, move_y);
	gtk_window_set_icon_from_file(GTK_WINDOW(main_window), "/usr/share/icons/ubuntu-mono-dark/status/24/gnome-netstatus-tx.svg", NULL);

	// Layout.
	pVBox = gtk_vbox_new(FALSE, 0);
	apacheHBox = gtk_hbox_new(FALSE, 0); // FALSE : non equivalent space for all included widgets
	mysqlHBox = gtk_hbox_new(FALSE, 0);

	gtk_container_add(GTK_CONTAINER(main_window),pVBox);

	status_bar = gtk_statusbar_new();
	gtk_box_pack_end(GTK_BOX(pVBox), status_bar, FALSE, FALSE, 10);
	
	// Big title label.
	tmp_txt = g_locale_to_utf8("<span size=\"xx-large\"><b>Servers Control Panel</b></span>\n", -1, NULL, NULL, NULL);
	title_lbl = gtk_label_new(tmp_txt);
	g_free(tmp_txt);
	gtk_label_set_use_markup(GTK_LABEL(title_lbl), TRUE);
	gtk_label_set_justify(GTK_LABEL(title_lbl), GTK_JUSTIFY_CENTER);

	gtk_box_pack_start(GTK_BOX(pVBox), title_lbl, FALSE, FALSE, 0);
	
	// Localhost button.
	localhost_button = gtk_button_new_from_stock("http://localhost");
	gtk_box_pack_start(GTK_BOX(pVBox), localhost_button, TRUE, TRUE, 10);
	
	// Apache label.
	refresh_apache_lbl();

	// Apache button.
	apache_button = gtk_button_new_from_stock("switch");
	gtk_box_pack_start(GTK_BOX (apacheHBox), apache_button, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(pVBox), apacheHBox, TRUE, TRUE, 0);

	
	// Mysql label.
	refresh_mysql_lbl();
	
	// Mysql button.
	mysql_button = gtk_button_new_from_stock("switch");
	gtk_box_pack_start(GTK_BOX (mysqlHBox), mysql_button, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(pVBox), mysqlHBox, TRUE, TRUE, 0);


	// Bindings.
	g_signal_connect(G_OBJECT(main_window), "delete-event", G_CALLBACK(gtk_main_quit), NULL);
	
	g_signal_connect(G_OBJECT(localhost_button), "enter", G_CALLBACK(on_localhost_button_hover), NULL);
	g_signal_connect(G_OBJECT(localhost_button), "clicked", G_CALLBACK(on_localhost_button_clicked), NULL);
	g_signal_connect(G_OBJECT(localhost_button), "leave", G_CALLBACK(clear_status_bar), NULL);
	
	g_signal_connect(G_OBJECT(apache_button), "enter", G_CALLBACK(on_apache_button_hover), NULL);
	g_signal_connect(G_OBJECT(apache_button), "clicked", G_CALLBACK(on_apache_button_clicked), NULL);
	g_signal_connect(G_OBJECT(apache_button), "leave", G_CALLBACK(clear_status_bar), NULL);
	
	g_signal_connect(G_OBJECT(mysql_button), "enter", G_CALLBACK(on_mysql_button_hover), NULL);
	g_signal_connect(G_OBJECT(mysql_button), "clicked", G_CALLBACK(on_mysql_button_clicked), NULL);
	g_signal_connect(G_OBJECT(mysql_button), "leave", G_CALLBACK(clear_status_bar), NULL);

	on_mysql_button_clicked(mysql_button,NULL);
	on_apache_button_clicked(apache_button,NULL);

	gtk_widget_show_all(main_window); // On affiche 'main_window' et tout ce qu'il contient

	gtk_main();

	return EXIT_SUCCESS;

}

int check_apache_state() {
	#ifdef DEBUG
		system("if [ `service apache2 status | grep -r NOT | wc -l` -eq 0 ]; then echo 'apache running'; else echo 'apache stopped';fi");
	#endif
	
	if (WEXITSTATUS(system("if [ `service apache2 status | grep -r NOT | wc -l` -eq 0 ]; then exit 1; else exit 0;fi")) == 1)
		return 1;
	return 0;
}

int check_mysql_state() {
	#ifdef DEBUG
		system("if [ `service mysql status | grep -r stop | wc -l` -eq 0 ]; then echo 'mysql running'; else echo 'mysql stopped';fi");
	#endif
	
	if (WEXITSTATUS(system("if [ `service mysql status | grep -r stop | wc -l` -eq 0 ]; then exit 1; else exit 0;fi")) == 1)
		return 1;
	return 0;
}

void on_apache_button_hover(GtkWidget *pButton, gpointer iContextId)
{
    gtk_statusbar_push (GTK_STATUSBAR (status_bar), GPOINTER_TO_INT(iContextId), "Switch apache status...");
}

int on_apache_button_clicked(GtkWidget *pButton, gpointer iContextId)
{
    #ifdef DEBUG
    	system("echo on_apache_button_clicked before");
		check_apache_state();
	#endif
	
	if (apache_status == 0)
		system("gksudo service apache2 start");
	else
		system("gksudo service apache2 stop");
	
	refresh_apache_lbl();
	
	#ifdef DEBUG
	   	system("echo on_apache_button_clicked after");
		check_apache_state();
	#endif
	
	return (apache_status = check_apache_state());
}

void on_mysql_button_hover(GtkWidget *pButton, gpointer iContextId)
{
    gtk_statusbar_push (GTK_STATUSBAR (status_bar), GPOINTER_TO_INT(iContextId), "Switch mysql status...");
}

int on_mysql_button_clicked(GtkWidget *pButton, gpointer iContextId)
{
    #ifdef DEBUG
    	system("echo on_mysql_button_clicked before");
		check_mysql_state();
	#endif
	
	if (mysql_status == 0)
		system("gksudo service mysql start");
	else
		system("gksudo service mysql stop");
		
	refresh_mysql_lbl();
	
	#ifdef DEBUG
	   	system("echo on_mysql_button_clicked after");
		check_mysql_state();
	#endif
	
	return (mysql_status = check_mysql_state());
}

void clear_status_bar(GtkWidget *pButton, gpointer iContextId)
{
    gtk_statusbar_pop(GTK_STATUSBAR(status_bar), GPOINTER_TO_INT(iContextId));
}

void on_localhost_button_hover(GtkWidget *pButton, gpointer iContextId)
{
	gtk_statusbar_push (GTK_STATUSBAR (status_bar), GPOINTER_TO_INT(iContextId), "Open localhost in your default browser...");
}

void on_localhost_button_clicked(GtkWidget *pButton, gpointer iContextId)
{
	system("x-www-browser http://localhost &");
}

void refresh_apache_lbl(){
	gchar* tmp_txt = NULL;

	if ((apache_status = check_apache_state()) == 1)
		tmp_txt = g_locale_to_utf8("<span size=\"x-large\"><b>apache\t <span color=\"green\">ON\t</span></b></span>", -1, NULL, NULL, NULL);
	else
		tmp_txt = g_locale_to_utf8("<span size=\"x-large\"><b>apache\t<span color=\"red\">OFF\t</span></b></span>", -1, NULL, NULL, NULL);
	
	if (GTK_IS_WIDGET(apache_lbl))
		gtk_label_set_label(GTK_LABEL(apache_lbl), tmp_txt);
	else {
		apache_lbl = gtk_label_new(tmp_txt);
		gtk_label_set_use_markup(GTK_LABEL(apache_lbl), TRUE);
		gtk_label_set_justify(GTK_LABEL(apache_lbl), GTK_JUSTIFY_CENTER);
		gtk_box_pack_start(GTK_BOX(apacheHBox), apache_lbl, FALSE, FALSE, 20);
	}
	g_free(tmp_txt);
}

void refresh_mysql_lbl(){
	gchar* tmp_txt = NULL;

	if ((mysql_status = check_mysql_state()) == 1)
		tmp_txt = g_locale_to_utf8("<span size=\"x-large\"><b>mysql\t <span color=\"green\">ON\t</span></b></span>", -1, NULL, NULL, NULL);
	else
		tmp_txt = g_locale_to_utf8("<span size=\"x-large\"><b>mysql\t<span color=\"red\">OFF\t</span></b></span>", -1, NULL, NULL, NULL);
	
	if (GTK_IS_WIDGET(mysql_lbl))
		gtk_label_set_label(GTK_LABEL(mysql_lbl), tmp_txt);
	else {
		mysql_lbl = gtk_label_new(tmp_txt);
		gtk_label_set_use_markup(GTK_LABEL(mysql_lbl), TRUE);
		gtk_label_set_justify(GTK_LABEL(mysql_lbl), GTK_JUSTIFY_CENTER);
		gtk_box_pack_start(GTK_BOX(mysqlHBox), mysql_lbl, FALSE, FALSE, 20);
	}
	g_free(tmp_txt);
}

