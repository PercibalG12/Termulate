#include <gtk/gtk.h>
#include <string.h>

#include "commands.h"

GtkWidget *text_view;

void appendOutput(const char *text)
{
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    GtkTextIter end;

    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, text, -1);

    GtkTextMark *mark = gtk_text_buffer_get_insert(buffer);
    gtk_text_view_scroll_mark_onscreen(GTK_TEXT_VIEW(text_view), mark);
}

void clearScreen(void)
{
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_text_buffer_set_text(buffer, "", -1);
}

static char *get_current_line_text(void)
{
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    GtkTextIter start, end;

    gtk_text_buffer_get_iter_at_mark(buffer, &end, gtk_text_buffer_get_insert(buffer));
    start = end;

    gtk_text_iter_set_line_offset(&start, 0);

    return gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
}

gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
    if (event->keyval == GDK_KEY_Return)
    {
        char *line = get_current_line_text();

        char *prompt = strrchr(line, '>');
        char *input = prompt ? prompt + 1 : line;

        while (*input == ' ')
        {
            input++;
        }

        appendOutput("\n");

        if (strcmp(input, "clear") == 0)
        {
            clearScreen();
            appendOutput("Termulate> ");
        }
        else if (strcmp(input, "exit") == 0)
        {
            appendOutput("Exiting Termulate...\n");
            gtk_main_quit();
        }
        else
        {
            processCommand(input, appendOutput);
        }

        g_free(line);

        return TRUE;
    }

    return FALSE;
}

void apply_dark_theme(void)
{
    GtkCssProvider *provider = gtk_css_provider_new();

    gtk_css_provider_load_from_data(
        provider,
        "window {"
        "   background-color: #0c0c0c;"
        "}"
        "textview {"
        "   background-color: #0c0c0c;"
        "   color: #00ff66;"
        "   font-family: Consolas, monospace;"
        "   font-size: 15px;"
        "}"
        "textview text {"
        "   background-color: #0c0c0c;"
        "   color: #00ff66;"
        "   caret-color: #00ff66;"
        "}"
        "scrolledwindow {"
        "   background-color: #0c0c0c;"
        "}",
        -1,
        NULL);

    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    g_object_unref(provider);
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    GtkWidget *window;
    GtkWidget *scroll;

    apply_dark_theme();

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Termulate");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 500);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(window), scroll);

    text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), TRUE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), TRUE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD_CHAR);

    gtk_container_add(GTK_CONTAINER(scroll), text_view);

    g_signal_connect(text_view, "key-press-event", G_CALLBACK(on_key_press), NULL);

    appendOutput("Welcome to Termulate\n");
    appendOutput("\n");
appendOutput(
"████████╗███████╗██████╗ ███╗   ███╗██╗   ██╗██╗      █████╗ ████████╗███████╗\n"
"╚══██╔══╝██╔════╝██╔══██╗████╗ ████║██║   ██║██║     ██╔══██╗╚══██╔══╝██╔════╝\n"
"   ██║   █████╗  ██████╔╝██╔████╔██║██║   ██║██║     ███████║   ██║   █████╗\n"
"   ██║   ██╔══╝  ██╔══██╗██║╚██╔╝██║██║   ██║██║     ██╔══██║   ██║   ██╔══╝\n"
"   ██║   ███████╗██║  ██║██║ ╚═╝ ██║╚██████╔╝███████╗██║  ██║   ██║   ███████╗\n"
"   ╚═╝   ╚══════╝╚═╝  ╚═╝╚═╝     ╚═╝ ╚═════╝ ╚══════╝╚═╝  ╚═╝   ╚═╝   ╚══════╝\n"
);
appendOutput("\n");
    appendOutput("Type 'help' to see available commands.\n\n");
    appendOutput("Termulate> ");

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}