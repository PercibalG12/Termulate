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
            input++;

        appendOutput("\n");

        if (strcmp(input, "clear") == 0)
        {
            clearScreen();
            appendOutput("C:\\Users\\perci> ");
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
    appendOutput("Welcome to Termulate\n\n");

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

GtkWidget *create_tab_label(const char *title)
{
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);

    GtkWidget *icon = gtk_label_new("▣");
    GtkWidget *label = gtk_label_new(title);
    GtkWidget *close = gtk_label_new("×");

    gtk_style_context_add_class(gtk_widget_get_style_context(box), "tab-box");
    gtk_style_context_add_class(gtk_widget_get_style_context(icon), "tab-icon");
    gtk_style_context_add_class(gtk_widget_get_style_context(label), "tab-title");
    gtk_style_context_add_class(gtk_widget_get_style_context(close), "tab-close");

    gtk_box_pack_start(GTK_BOX(box), icon, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), close, FALSE, FALSE, 6);

    gtk_widget_show_all(box);

    return box;
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

    if (session_count == 1)
        snprintf(tab_name, sizeof(tab_name), "Session");
    else
        snprintf(tab_name, sizeof(tab_name), "Session %d", session_count);

    GtkWidget *tab_label = create_tab_label(tab_name);

    int page_num = gtk_notebook_append_page(GTK_NOTEBOOK(notebook), scroll, tab_label);
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
        "   background-color: #0c0c0c;"
        "}"

        "notebook {"
        "   background-color: #0c0c0c;"
        "   border: none;"
        "}"

        "notebook header {"
        "   background-color: #2b2b2b;"
        "   border: none;"
        "   min-height: 34px;"
        "   padding: 0;"
        "}"

        "notebook tabs {"
        "   background-color: #2b2b2b;"
        "   min-height: 34px;"
        "}"

        "notebook tab {"
        "   background: #1f1f1f;"
        "   color: #f1f1f1;"
        "   padding: 5px 12px;"
        "   border-radius: 6px 6px 0 0;"
        "   margin: 3px 2px 0 4px;"
        "   border: none;"
        "   min-height: 28px;"
        "}"

        "notebook tab:hover {"
        "   background: #252525;"
        "}"

        "notebook tab:checked {"
        "   background: #0c0c0c;"
        "   color: #ffffff;"
        "   border-bottom: 2px solid #4da3ff;"
        "}"

        ".tab-box {"
        "   background: transparent;"
        "   padding: 0;"
        "}"

        ".tab-icon {"
        "   color: #ffffff;"
        "   font-size: 10px;"
        "}"

        ".tab-title {"
        "   color: #ffffff;"
        "   font-family: Segoe UI, Arial, sans-serif;"
        "   font-size: 12px;"
        "}"

        ".tab-close {"
        "   color: #ffffff;"
        "   font-size: 14px;"
        "}"

        ".tab-close:hover {"
        "   color: #ff6b6b;"
        "}"

        ".plus-button {"
        "   background: #2b2b2b;"
        "   color: #ffffff;"
        "   border: none;"
        "   font-size: 16px;"
        "   padding: 2px 10px;"
        "   min-height: 28px;"
        "   min-width: 34px;"
        "   box-shadow: none;"
        "}"

        ".plus-button:hover {"
        "   background: #3a3a3a;"
        "}"

        "textview {"
        "   background-color: #0c0c0c;"
        "   color: #cccccc;"
        "   font-family: Consolas, monospace;"
        "   font-size: 15px;"
        "   padding: 8px;"
        "}"

        "textview text {"
        "   background-color: #0c0c0c;"
        "   color: #cccccc;"
        "   caret-color: #ffffff;"
        "}"

        "scrolledwindow {"
        "   background-color: #0c0c0c;"
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
    GtkWidget *plus_button;

    apply_dark_theme();

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Termulate");
    gtk_window_set_default_size(GTK_WINDOW(window), 1100, 650);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    notebook = gtk_notebook_new();
    gtk_notebook_set_scrollable(GTK_NOTEBOOK(notebook), TRUE);
    gtk_box_pack_start(GTK_BOX(main_box), notebook, TRUE, TRUE, 0);

    plus_button = gtk_button_new_with_label("+");
    gtk_style_context_add_class(
        gtk_widget_get_style_context(plus_button),
        "plus-button"
    );

    gtk_notebook_set_action_widget(
        GTK_NOTEBOOK(notebook),
        plus_button,
        GTK_PACK_END
    );

    g_signal_connect(
        plus_button,
        "clicked",
        G_CALLBACK(create_new_session),
        NULL
    );

    create_new_session(NULL, NULL);

    gtk_widget_show_all(window);
    gtk_widget_show(plus_button);

    gtk_main();

    return 0;
}