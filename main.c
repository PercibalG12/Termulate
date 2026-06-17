#include <gtk/gtk.h>
#include <string.h>

#include "commands.h"

GtkWidget *text_view;
GtkWidget *notebook;

int session_count = 0;

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

static char *get_current_line_text(GtkWidget *current_text_view)
{
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(current_text_view));
    GtkTextIter start, end;

    gtk_text_buffer_get_iter_at_mark(buffer, &end, gtk_text_buffer_get_insert(buffer));
    start = end;

    gtk_text_iter_set_line_offset(&start, 0);

    return gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
}

gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
    text_view = widget;

    if (event->keyval == GDK_KEY_Return)
    {
        char *line = get_current_line_text(widget);

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
            appendOutput("Exiting this Termulate session...\n");
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

void add_welcome_message(void)
{
    appendOutput("Welcome to Termulate GUI\n\n");

    appendOutput(
"████████╗███████╗██████╗ ███╗   ███╗██╗   ██╗██╗      █████╗ ████████╗███████╗\n"
"╚══██╔══╝██╔════╝██╔══██╗████╗ ████║██║   ██║██║     ██╔══██╗╚══██╔══╝██╔════╝\n"
"   ██║   █████╗  ██████╔╝██╔████╔██║██║   ██║██║     ███████║   ██║   █████╗\n"
"   ██║   ██╔══╝  ██╔══██╗██║╚██╔╝██║██║   ██║██║     ██╔══██║   ██║   ██╔══╝\n"
"   ██║   ███████╗██║  ██║██║ ╚═╝ ██║╚██████╔╝███████╗██║  ██║   ██║   ███████╗\n"
"   ╚═╝   ╚══════╝╚═╝  ╚═╝╚═╝     ╚═╝ ╚═════╝ ╚══════╝╚═╝  ╚═╝   ╚═╝   ╚══════╝\n"
    );

    appendOutput("\nType 'help' to see available commands.\n\n");
    appendOutput("Termulate> ");
}

void create_new_session(GtkWidget *button, gpointer data)
{
    session_count++;

    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    GtkWidget *new_text_view = gtk_text_view_new();

    gtk_text_view_set_editable(GTK_TEXT_VIEW(new_text_view), TRUE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(new_text_view), TRUE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(new_text_view), GTK_WRAP_WORD_CHAR);

    gtk_container_add(GTK_CONTAINER(scroll), new_text_view);

    g_signal_connect(new_text_view, "key-press-event", G_CALLBACK(on_key_press), NULL);

    char tab_name[50];
    snprintf(tab_name, sizeof(tab_name), "Session %d", session_count);

    GtkWidget *label = gtk_label_new(tab_name);

    int page_num = gtk_notebook_append_page(GTK_NOTEBOOK(notebook), scroll, label);
    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), page_num);

    text_view = new_text_view;
    add_welcome_message();

    gtk_widget_show_all(scroll);
}

void apply_dark_theme(void)
{
    GtkCssProvider *provider = gtk_css_provider_new();

    gtk_css_provider_load_from_data(
        provider,

        "window {"
        "   background-color: #0b0f10;"
        "}"

        ".top-bar {"
        "   background-color: #111718;"
        "   padding: 6px;"
        "   border-bottom: 1px solid #1f2a2c;"
        "}"

        ".app-title {"
        "   color: #00ff88;"
        "   font-family: Consolas, monospace;"
        "   font-size: 15px;"
        "   font-weight: bold;"
        "   margin-left: 8px;"
        "}"

        ".plus-button {"
        "   background: #1b2527;"
        "   color: #00ff88;"
        "   border-radius: 8px;"
        "   border: 1px solid #263638;"
        "   padding: 4px 12px;"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "}"

        ".plus-button:hover {"
        "   background: #243133;"
        "}"

        "notebook {"
        "   background-color: #0b0f10;"
        "   border: none;"
        "}"

        "notebook tab {"
        "   background: #111718;"
        "   color: #8cffc1;"
        "   padding: 7px 14px;"
        "   border-radius: 8px 8px 0 0;"
        "   margin: 2px;"
        "}"

        "notebook tab:checked {"
        "   background: #182224;"
        "   color: #00ff88;"
        "   border-bottom: 2px solid #00ff88;"
        "}"

        "textview {"
        "   background-color: #0b0f10;"
        "   color: #00ff88;"
        "   font-family: Consolas, monospace;"
        "   font-size: 15px;"
        "   padding: 12px;"
        "}"

        "textview text {"
        "   background-color: #0b0f10;"
        "   color: #00ff88;"
        "   caret-color: #00ff88;"
        "}"

        "scrolledwindow {"
        "   background-color: #0b0f10;"
        "   border: none;"
        "}",

        -1,
        NULL
    );

    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );

    g_object_unref(provider);
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    GtkWidget *window;
    GtkWidget *main_box;
    GtkWidget *top_bar;
    GtkWidget *plus_button;

    apply_dark_theme();

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Termulate GUI");
    gtk_window_set_default_size(GTK_WINDOW(window), 900, 550);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    top_bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_style_context_add_class(gtk_widget_get_style_context(top_bar), "top-bar");
    gtk_box_pack_start(GTK_BOX(main_box), top_bar, FALSE, FALSE, 0);

    GtkWidget *title = gtk_label_new("Termulate");
    gtk_style_context_add_class(gtk_widget_get_style_context(title), "app-title");
    gtk_box_pack_start(GTK_BOX(top_bar), title, FALSE, FALSE, 8);

    plus_button = gtk_button_new_with_label("+");
    gtk_style_context_add_class(gtk_widget_get_style_context(plus_button), "plus-button");
    gtk_box_pack_end(GTK_BOX(top_bar), plus_button, FALSE, FALSE, 8);

    notebook = gtk_notebook_new();
    gtk_box_pack_start(GTK_BOX(main_box), notebook, TRUE, TRUE, 0);

    g_signal_connect(plus_button, "clicked", G_CALLBACK(create_new_session), NULL);

    create_new_session(NULL, NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}