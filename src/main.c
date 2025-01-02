#include <gtk/gtk.h>
#include <glib/gstdio.h>
#include <time.h>

static void
print_hello(GtkWidget *widget,
            gpointer data)
{
  g_print("Hello World\n");
}

static void
quit_cb(GtkWindow *window)
{
  gtk_window_close(window);
}

static void
change_label_property(GtkWidget *widget,
                      gpointer data)
{
  GtkLabel *label = GTK_LABEL(data);
  gtk_label_set_text(label, "New Text");
  gtk_widget_set_name(GTK_WIDGET(label), "new_name");
}

static gboolean
update_time(gpointer user_data)
{
  GtkLabel *label = GTK_LABEL(user_data);
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  char time_str[9];
  strftime(time_str, sizeof(time_str), "%H:%M:%S", t);
  gtk_label_set_text(label, time_str);
  return TRUE; // Continue calling this function
}

static gboolean
update_date(gpointer user_data)
{
  GtkLabel *label = GTK_LABEL(user_data);
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  char date_str[11];
  strftime(date_str, sizeof(date_str), "%Y-%m-%d", t);
  gtk_label_set_text(label, date_str);
  return TRUE; // Continue calling this function
}

static void
activate(GtkApplication *app,
         gpointer user_data)
{
  /* Construct a GtkBuilder instance and load our UI description */
  GtkBuilder *builder;
  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "/Users/banky/Documents/GitHub/control-board/src/builder.ui", NULL);

  /* Connect signal handlers to the constructed widgets. */
  GObject *window;
  window = gtk_builder_get_object(builder, "window");
  gtk_window_set_application(GTK_WINDOW(window), app);
  gtk_window_fullscreen(GTK_WINDOW(window));
  gtk_grid_set_row_homogeneous(GTK_GRID(gtk_builder_get_object(builder, "grid")), TRUE);
  gtk_grid_set_column_homogeneous(GTK_GRID(gtk_builder_get_object(builder, "grid")), TRUE);

  GtkCssProvider *cssprovider;
  cssprovider = gtk_css_provider_new();
  gtk_css_provider_load_from_path(cssprovider, "/Users/banky/Documents/GitHub/control-board/src/styles/style.css");
  gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(cssprovider), GTK_STYLE_PROVIDER_PRIORITY_USER);
/*
  GObject *button;
  button = gtk_builder_get_object(builder, "button1");
  g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);

  button = gtk_builder_get_object(builder, "button2");
  g_signal_connect(button, "clicked", G_CALLBACK(change_label_property), NULL);

  button = gtk_builder_get_object(builder, "quit");
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(quit_cb), window);
*/

  gtk_widget_set_visible(GTK_WIDGET(window), TRUE);
  /* Get the clock label object and set up a timer to update it */
  GObject *time_label;
  time_label = gtk_builder_get_object(builder, "time_label");
  g_timeout_add_seconds(1, update_time, time_label);

  GObject *date_label;
  date_label = gtk_builder_get_object(builder, "date_label");
  g_timeout_add_seconds(1, update_date, date_label);

  /* We do not need the builder any more */
  g_object_unref(builder);
}

int main(int argc,
         char *argv[])
{
#ifdef GTK_SRCDIR
  g_chdir(GTK_SRCDIR);
#endif

  GtkApplication *app = gtk_application_new("org.iambanky.controlboard", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

  int status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}
