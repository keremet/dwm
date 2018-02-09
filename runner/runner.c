#include <gtk/gtk.h>
#include <stdlib.h>
#include <unistd.h>

static void run_command(GtkMenuItem *item, char** command)
{
	if (fork() == 0) {
		setsid();
		execvp(command[0], command);
		fprintf(stderr, "runner: execvp %s", command[0]);
		perror(" failed");
		exit(0);
	}
}

static void trayIconActivated(GObject *trayIcon, gpointer popUpMenu)
{
	gtk_menu_popup(GTK_MENU(popUpMenu), NULL, NULL, NULL, NULL, 0, gtk_get_current_event_time());
}

struct MENU_MODEL{
	const char* name;
	const char* command[2];
	struct MENU_MODEL* mm;
};

static struct MENU_MODEL MM_PROGRAMMING[] = {
	{"Geany", {"geany", 0}},
	{0},
};

static struct MENU_MODEL MM_MAIN_LEFT[] = {
	{"GEdit", {"gedit", 0}},
	{"xterm", {"xterm", 0}},
	{"Программирование", {0}, MM_PROGRAMMING},
	{0},
};

static GtkWidget *createMenu(struct MENU_MODEL *MM)
{
	GtkWidget *menu = gtk_menu_new();
	for(; MM->name; MM++){
		GtkWidget *mi = gtk_menu_item_new_with_label (MM->name);
		if(MM->mm){
			gtk_menu_item_set_submenu (GTK_MENU_ITEM (mi), createMenu (MM->mm));
		}else{
			g_signal_connect (G_OBJECT (mi), "activate", G_CALLBACK (run_command), MM->command);
		}
		gtk_menu_shell_append (GTK_MENU_SHELL (menu), mi);
	}
	gtk_widget_show_all (menu);
	return menu;
}

int main(int argc, char *argv[])
{
	gtk_init (&argc, &argv);
	extern const char* logo_xpm[];
	GtkStatusIcon *trayIcon = gtk_status_icon_new_from_pixbuf (gdk_pixbuf_new_from_xpm_data (logo_xpm));
	gtk_status_icon_set_tooltip (trayIcon, "Запуск программ");
	g_signal_connect(GTK_STATUS_ICON (trayIcon), "activate", GTK_SIGNAL_FUNC (trayIconActivated), (gpointer)createMenu(MM_MAIN_LEFT));
	gtk_status_icon_set_visible(trayIcon, TRUE);

	gtk_main ();
	return 0;
}

/*
при щелчке правой кнопкой можно сделать еще одно меню - более подробное или наоборот
    g_signal_connect(GTK_STATUS_ICON (trayIcon), "popup-menu", GTK_SIGNAL_FUNC (trayIconPopup), menu);
static void trayIconPopup(GtkStatusIcon *status_icon, guint button, guint32 activate_time, gpointer popUpMenu)
{
    gtk_menu_popup(GTK_MENU(popUpMenu), NULL, NULL, gtk_status_icon_position_menu, status_icon, button, activate_time);
}
*/