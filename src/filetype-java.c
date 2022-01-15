#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#ifndef PLAT_GTK
#   define PLAT_GTK 1
#endif

#include <gtk/gtk.h>

#include "cssedwindow.h"
#include "document.h"
#include "file-type-manager.h"
#include "support.h"
#include "debug.h"
#include "utils.h"

#include <Scintilla.h>
#include <SciLexer.h>
#include <ScintillaWidget.h>

void
java_filetype_char_added(CssedDoc* doc, gchar  lastchar)
{
	if( lastchar == '\n')
		document_indent_as_last_line(doc);
}

void
filetype_java_set_style(CssedDoc* doc, GtkWidget* editor)
{
	ScintillaObject * sci;
	CssedWindow* window;
	CssedConfig* cfg;
	GtkWidget *scintilla;

	const gchar primaryKeyWords[] = "boolean break byte case catch char const continue \
debugger default delete do double else enum false \
final finally float for goto if in \
instanceof int long native new null  private \
protected public return short static super switch synchronized \
this throw throws transient true try typeof var void volatile \
while with";

	const gchar secondaryKeyWords[] = "abstract continue for new switch assert default package synchronized boolean \
do if implements private this break  double implements protected throw byte else import public throws case enum instanceof \
return transient catch extends int short try char final interface static void class finally long strictfp volatile \
float native super while" ;

	const gchar docCommentKeyWords[] = "";

	const gchar typedefsKeyWords[] =  "";
/*	Those are Java 1.1 Classes

"AWTError AWTEvent AWTEventMulticaster AWTException AbstractMethodError \
AccessException Acl AclEntry AclNotFoundException ActionEvent ActionListener Adjustable AdjustmentEvent AdjustmentListener \
Adler32 AlreadyBoundException Applet AppletContext AppletStub AreaAveragingScaleFilter ArithmeticException Array \
ArrayIndexOutOfBoundsException ArrayStoreException AudioClip BeanDescriptor BeanInfo Beans BigDecimal BigInteger \
BindException BitSet Boolean BorderLayout BreakIterator BufferedOutputStream BufferedReader BufferedWriter Button ButtonPeer \
Byte ByteArrayInputStream ByteArrayOutputStream CRC32 Calendar CallableStatement Canvas CanvasPeer CardLayout Certificate \
CharArrayReader CharArrayWriter CharConversionException Character CharacterIterator Checkbox CheckboxGroup CheckboxMenuItem \
CheckboxMenuItemPeer CheckboxPeer CheckedInputStream CheckedOutputStream Checksum Choice ChoiceFormat ChoicePeer Class \
ClassCastException ClassCircularityError ClassFormatError ClassLoader ClassNotFoundException Clipboard\
ClipboardOwner Cloneable CloneNotSupportedException CollationElementIterator CollationKey Collator Color ColorModel Compiler \
Component ComponentAdapter ComponentEvent ComponentListener ComponentPeer ConnectException ConnectException ConnectIOException \
Connection Constructor Container ContainerAdapter ContainerEvent ContainerListener ContainerPeer ContentHandler \
ContentHandlerFactory CropImageFilter Cursor Customizer DGC DSAKey DSAKeyPairGenerator DSAParams DSAPrivateKey DSAPublicKey \
DataFlavor DataFormatException DataInput DataInputStream DataOutput DataOutputStream DataTruncation DatabaseMetaData \
DatagramPacket DatagramSocket DatagramSocketImpl Date Date DateFormat DateFormatSymbols DecimalFormat DecimalFormatSymbols \
Deflater DeflaterOutputStream Dialog DialogPeer Dictionary DigestException DigestInputStream DigestOutputStream Dimension \
DirectColorModel Double Driver DriverManager DriverPropertyInfo EOFException EmptyStackException Enumeration Error Error Index \
Error Index Event EventListener EventObject EventQueue EventSetDescriptor Exception Exception Index \
ExceptionInInitializerError ExportException Externalizable FeatureDescriptor Field FieldPosition File FileDescriptor \
FileDialog FileDialogPeer FileInputStream FileNameMap FileNotFoundException FileOutputStream FileReader FileWriter \
FilenameFilter FilterInputStream FilterOutputStream FilterReader FilterWriter FilteredImageSource Float FlowLayout \
FocusAdapter FocusEvent FocusListener Font FontMetrics FontPeer Format FormatException Frame FramePeer GZIPInputStream \
GZIPOutputStream Graphics GregorianCalendar GridBagConstraints GridBagLayout GridLayout Group Hashtable HttpURLConnection \
IOException Identity IdentityScope IllegalAccessError IllegalAccessException IllegalArgumentException \
IllegalComponentStateException IllegalMonitorStateException IllegalStateException IllegalThreadStateException Image \
ImageConsumer ImageFilter ImageObserver ImageProducer IncompatibleClassChangeError IndexColorModel IndexOutOfBoundsException \
IndexedPropertyDescriptor InetAddress Inflater InflaterInputStream InputEvent InputStream InputStreamReader Insets \
InstantiationError InstantiationException Integer InternalError InterruptedException InterruptedIOException \
IntrospectionException Introspector InvalidClassException InvalidKeyException InvalidObjectException InvalidParameterException \
InvocationTargetException ItemEvent ItemListener ItemSelectable Key KeyAdapter KeyEvent KeyException KeyListener \
KeyManagementException KeyPair KeyPairGenerator Label LabelPeer LastOwnerException LayoutManager LayoutManager2 Lease \
LightweightPeer LineNumberInputStream LineNumberReader LinkageError List ListPeer ListResourceBundle LoaderHandler Locale \
LocateRegistry LogStream Long MalformedURLException MarshalException Math MediaTracker Member MemoryImageSource Menu MenuBar \
MenuBarPeer MenuComponent MenuComponentPeer MenuContainer MenuItem MenuItemPeer MenuPeer MenuShortcut MessageDigest \
MessageFormat Method MethodDescriptor MissingResourceException Modifier MouseAdapter MouseEvent MouseListener \
MouseMotionAdapter MouseMotionListener MulticastSocket Naming NegativeArraySizeException \
NoClassDefFoundError NoRouteToHostException NoSuchAlgorithmException NoSuchElementException NoSuchFieldError \
NoSuchFieldException NoSuchMethodError NoSuchMethodException NoSuchObjectException NoSuchProviderException NotActiveException \
NotBoundException NotOwnerException NotSerializableException NullPointerException Number NumberFormat NumberFormatException \
ObjID Object ObjectInput ObjectInputStream ObjectInputValidation ObjectOutput ObjectOutputStream ObjectStreamClass \
ObjectStreamException Observable Observer Operation OptionalDataException OutOfMemoryError OutputStream OutputStreamWriter \
Owner PaintEvent Panel PanelPeer ParameterDescriptor ParseException ParsePosition Permission PipedInputStream \
PipedOutputStream PipedReader PipedWriter PixelGrabber Point Polygon PopupMenu PopupMenuPeer PreparedStatement Principal \
PrintGraphics PrintJob PrintStream PrintWriter PrivateKey Process Properties PropertyChangeEvent PropertyChangeListener \
PropertyChangeSupport PropertyDescriptor PropertyEditor PropertyEditorManager PropertyEditorSupport PropertyResourceBundle \
PropertyVetoException ProtocolException Provider ProviderException PublicKey PushbackInputStream PushbackReader \
RGBImageFilter RMIClassLoader RMIFailureHandler RMISecurityException RMISecurityManager RMISocketFactory Random \
RandomAccessFile Reader Rectangle Registry RegistryHandler Remote RemoteCall RemoteException RemoteObject RemoteRef \
RemoteServer RemoteStub ReplicateScaleFilter ResourceBundle ResultSet ResultSetMetaData RuleBasedCollator Runnable Runtime \
RuntimeException SQLException SQLWarning ScrollPane ScrollPanePeer Scrollbar ScrollbarPeer SecureRandom Security \
SecurityException SecurityManager SequenceInputStream SerializableBufferedInputStream ServerCloneException ServerError \
ServerException ServerNotActiveException ServerRef ServerRuntimeException ServerSocket Shape Short Signature \
SignatureException Signer SimpleBeanInfo SimpleDateFormat SimpleTimeZone Skeleton SkeletonMismatchException \
SkeletonNotFoundException Socket SocketException SocketImpl SocketImplFactory SocketSecurityException Stack StackOverflowError \
Statement StreamCorruptedException StreamTokenizer String StringBuffer StringBufferInputStream StringCharacterIterator \
StringIndexOutOfBoundsException StringReader StringSelection StringTokenizer StringWriter StubNotFoundException \
SyncFailedException System SystemColor TextArea TextAreaPeer TextComponent TextComponentPeer TextEvent TextField TextFieldPeer \
TextListener Thread ThreadDeath ThreadGroup Throwable Time TimeZone Timestamp TooManyListenersException Toolkit Transferable \
Types UID URL URLConnection URLEncoder URLStreamHandler URLStreamHandlerFactory UTFDataFormatException UnexpectedException \
UnicastRemoteObject UnknownError UnknownHostException UnknownHostException UnknownServiceException UnmarshalException \
Unreferenced UnsatisfiedLinkError UnsupportedEncodingException UnsupportedFlavorException VMID Vector VerifyError \
VetoableChangeListener VetoableChangeSupport VirtualMachineError Visibility Void Window WindowAdapter WindowEvent \
WindowListener WindowPeer WriteAbortedException Writer ZipEntry ZipException ZipFile ZipInputStream ZipOutputStream ";
*/
	window = document_get_window(doc);
	cfg = cssed_window_get_config(window);
	
	if( editor ) scintilla = editor;
	else scintilla = document_get_scintilla_widget(doc);
	
	sci = SCINTILLA(scintilla);

	SSM (sci, SCI_SETPROPERTY, (sptr_t) "fold.preprocessor", (sptr_t) "1");
	SSM (sci, SCI_SETPROPERTY, (sptr_t) "fold.at.else", (sptr_t) "1");

	SSM (sci, SCI_STYLECLEARALL, 0, 0);
	SSM (sci, SCI_SETLEXER, SCLEX_CPP, 0);

	SSM(sci, SCI_SETKEYWORDS, 0, (sptr_t) primaryKeyWords);
	SSM(sci, SCI_SETKEYWORDS, 1, (sptr_t) secondaryKeyWords);
	SSM(sci, SCI_SETKEYWORDS, 2, (sptr_t) docCommentKeyWords);
	SSM(sci, SCI_SETKEYWORDS, 4, (sptr_t) typedefsKeyWords);

	SSM(sci, SCI_STYLESETFORE, SCE_C_DEFAULT, 0x000000);
	SSM(sci, SCI_STYLESETBACK, SCE_C_DEFAULT, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_COMMENT, 0x808080);
	SSM(sci, SCI_STYLESETBACK, SCE_C_COMMENT, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_COMMENTLINE, 0x808080);
	SSM(sci, SCI_STYLESETBACK, SCE_C_COMMENTLINE, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_COMMENTDOC, 0x808080);
	SSM(sci, SCI_STYLESETBACK, SCE_C_COMMENTDOC, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_NUMBER, 0x408000);
	SSM(sci, SCI_STYLESETBACK, SCE_C_NUMBER, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_WORD, 0x802080);
	SSM(sci, SCI_STYLESETBACK, SCE_C_WORD, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_WORD2, 0x800020);
	SSM(sci, SCI_STYLESETBACK, SCE_C_WORD2, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_STRING, 0x404000);
	SSM(sci, SCI_STYLESETBACK, SCE_C_STRING, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_CHARACTER, 0x004040);
	SSM(sci, SCI_STYLESETBACK, SCE_C_CHARACTER, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_UUID, 0x804040);
	SSM(sci, SCI_STYLESETBACK, SCE_C_UUID, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_PREPROCESSOR, 0x804060);
	SSM(sci, SCI_STYLESETBACK, SCE_C_PREPROCESSOR, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_OPERATOR, 0x101030);
	SSM(sci, SCI_STYLESETBACK, SCE_C_OPERATOR, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_IDENTIFIER, 0x100000);
	SSM(sci, SCI_STYLESETBACK, SCE_C_IDENTIFIER, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_STRINGEOL, 0x104010);
	SSM(sci, SCI_STYLESETBACK, SCE_C_STRINGEOL, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_VERBATIM, 0x906040);
	SSM(sci, SCI_STYLESETBACK, SCE_C_VERBATIM, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_REGEX, 0x905010);
	SSM(sci, SCI_STYLESETBACK, SCE_C_REGEX, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_COMMENTLINEDOC, 0x005020);
	SSM(sci, SCI_STYLESETBACK, SCE_C_COMMENTLINEDOC, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_COMMENTDOCKEYWORD, 0x400010);
	SSM(sci, SCI_STYLESETBACK, SCE_C_COMMENTDOCKEYWORD, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_COMMENTDOCKEYWORDERROR, 0x001020);
	SSM(sci, SCI_STYLESETBACK, SCE_C_COMMENTDOCKEYWORDERROR, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_GLOBALCLASS, 0x109040);
	SSM(sci, SCI_STYLESETBACK, SCE_C_GLOBALCLASS, 0xffffff);
}
/* ********************************** */

void
create_and_register_java_file_type (CssedWindow* window)
{
	CssedFileType* javafiletype;
	GtkWidget* javafiletype_menu;

	javafiletype_menu = gtk_menu_item_new_with_label("Java");

	javafiletype = cssed_file_type_new();
	javafiletype->char_added = java_filetype_char_added;

	javafiletype->modify_attemp_read_only = NULL;
	javafiletype->double_click = NULL;
	javafiletype->user_list_selection = NULL;
	javafiletype->dwell_start = NULL;
	javafiletype->dwell_end = NULL;
	javafiletype->hot_spot_click = NULL;
	javafiletype->hot_spot_doubleclick = NULL;
	javafiletype->call_tip_click = NULL;

	javafiletype->save_point_reached = NULL;
	javafiletype->save_point_left = NULL;
	javafiletype->uri_dropped = NULL;
	javafiletype->margin_click = NULL;
	javafiletype->modified = NULL;
	javafiletype->key_pressed = NULL;

	// configuraton and styling
	javafiletype->apply_style = filetype_java_set_style;

	// THOSE ARE NOT USED TEMPORALY
	javafiletype->get_config_page_widget = NULL;
	javafiletype->save_configuration_from_widget = NULL;	
	javafiletype->pop_menu = NULL;
	javafiletype->clean_popmenu = NULL;
	javafiletype->menu_item = javafiletype_menu;
	javafiletype->clean_menu = NULL;
	javafiletype->load_ui = NULL;

	// file related issues, those are mostly to avoid to open or save files
	javafiletype->open_file = NULL;
	javafiletype->save_doc = NULL;

	javafiletype->id = CSSED_FILETYPE_JAVA;
	javafiletype->style_min = SCE_C_DEFAULT;//(document_set_font_size)  (document_set_font_by_name) (Read/Write const only)
	javafiletype->style_max = SCE_C_GLOBALCLASS; //(document_set_font_size) (document_set_font_by_name)(Read/Write const only)
	javafiletype->can_fold = TRUE;
	javafiletype->label_language = g_strdup("Java"); // a name for the menu as CSS or XML
	javafiletype->lable_file = g_strdup("Java");

	cssed_file_type_add_pattern_spec(javafiletype, "*.java");
	cssed_file_type_menu_set_default_callback (window, javafiletype_menu, javafiletype);
	cssed_window_add_filetype (window, javafiletype, FALSE);
}
