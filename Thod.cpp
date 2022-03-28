// files used: history.txt rules.txt diary.txt vocabulary.txt agenda.txt sleeptime.txt alltime.txt menu.txt tips.txt time.cpp
#include "GoBang.cpp"
#include "time.cpp"
#include "Color.cpp"

int sleepStart, sleepEnd; // Global variable declaration

string currentTime(const char format[]){
  time_t now = time(0); // seconds from 1970
  char theTime[19];
  strftime (theTime, 19, format, localtime(&now)); 
  return theTime;
}
void createAlltime(){
  ofstream fout;
  fout.open("alltime.txt");
  
  int month, day;
  for (month = 1; month <= 12; month++){
    if (month == 1 || month == 3 || month == 5|| month == 7|| month == 8|| month == 10 || month == 12)
      for (day = 1; day <= 31; day++)
	    fout << month << "/" << day << " ";
	else if(month == 2)
	  for (day = 1; day <= 29; day++)
        fout << month << "/" << day << " ";
	else 
	  for (day = 1; day <= 30; day++)
		fout << month << "/" << day << " ";
  }
  
  for (int hour = 0; hour <= 12; hour++){
	for (int minute = 0; minute < 10; minute++)
      fout << hour << ":0" << minute << "am ";
	for (int minute = 10; minute < 60; minute++)
      fout << hour << ":" << minute << "am ";
  }
  
  for (int hour = 1; hour <= 12; hour++){
	for (int minute = 0; minute < 10; minute++)
      fout << hour << ":0" << minute << "pm ";
	for (int minute = 10; minute < 60; minute++)
      fout << hour << ":" << minute << "pm ";
  }
 
  fout.close();
  cout << "alltime.txt is created.";
}
bool existence(string fileName, string words){
  ifstream fin;
  fin.open(fileName.c_str());
  	
  while (fin.good()){ // line by line
	string aLine;
	getline(fin, aLine);
	if (words.length() == 0 && aLine.length() != 0) {fin.close(); return true;} // for the special case: (fileName, "")
	else if (words == aLine) {fin.close(); return true;} // match aLine
  }
  fin.close();
  
  fin.open(fileName.c_str());
  while (fin.good()){ // word by word
	string aWord;
    fin >> aWord;
	if (words == aWord) {fin.close(); return true;} // match aWord
  } 
 
  fin.close();
  return false;
}
bool keywords(string aSentence, const string aWord){
  // an error would occur if aSentence's length == aWord's length <= 3
  const int n = aWord.length();
  for (int i = 0; aSentence[i+n-1]; i++)
	if (aSentence.substr(i, n) == aWord)
	  return true;
  return false;
}
void getSleeptime(){
  ifstream fin;
  int n = count("history.txt", "lines");
    fin.open("history.txt");
    string lines[n];
    string info;
    for (int i = 0; fin.good(); i++) getline(fin, lines[i]);
    for (int i = n - 1; i >= 0; i--){
	  if (lines[i][0] == '&' && keywords(lines[i], "&Sleeptime")){
        info = lines[i];
	    break;
	  }
	}
  
  int locStart = info.find("from") + 5, locEnd = info.find("to") + 3;
  string start = info.substr(locStart, 4), end = info.substr(locEnd, 4);
  sleepStart = atoi(start.c_str()); sleepEnd = atoi(end.c_str());
  
  sleepEnd += 24;
  
  if (keywords(start, "am")) sleepStart += 24;
  else if (keywords(start, "pm")) sleepStart += 12;
}
static void validate(int input, int lowerBound, int upperBound) {
   if (input < lowerBound || input > upperBound) {
      string buf;
      puts("[INVALID]");
      cout << "Please enter a number between " << lowerBound << " and " << upperBound << ": ";
      cin >> buf; cin.ignore(1000, 10);
      input = atoi(buf.c_str());
      validate(input, lowerBound, upperBound);
  }
}

void eventAnalysis();
void backup();
void print(string); // print to the screen
void setSleeptime(); //sleeptime
string encrypt(string);
string decipher(string);
void encrypt_decipher(string); // encryption
void addVocab();
void vocab(); // voacb
void picViewer(); // pic
void txtOpener();
void diary(); // diary
bool CFN(); //CFN
void memo();
void viewMemos(); //memo
string menu(); // ?
void dialogue(string wantToDo);
void things(string); // wantToDo

void preamble(string& Time){
  color();
  
  if (!existence("alltime.txt", "")) createAlltime();
  if (!existence("history.txt", "&Sleeptime")) setSleeptime();
  
  int nEndl = 0;
  int d = atoi(currentTime("%d").c_str());
  
  /* if (currentTime("%B") == "January" && d == 1)
    {cout << "New Year\n"; nEndl++;} // New Year, 1/1
  if (currentTime("%B") == "January" && d >= 15 && d <= 21 && currentTime("%w") == "1")
    {cout << "Martin Luther King, Jr. Day\n"; nEndl++;} // Martin Luther King Day, third monday in January
  if (currentTime("%B") == "May" && d >= 25 && d <= 31 && currentTime("%w") == "1")
    {cout << "Memorial Day\n"; nEndl++;} // Memorial Day, last monday in May
  if (currentTime("%B") == "July" && d == 4)
    {cout << "Independence Day\n"; nEndl++;} // Independence Day, fourth of July 
  if (currentTime("%B") == "Septemper" && d >= 1 && d <= 7 && currentTime("%w") == "1")
    {cout << "Labor Day\n"; nEndl++;} // Labor Day, first Monday in Septemper
  if (currentTime("%B") == "November" && d == 11)
    {cout << "Veterans Day\n"; nEndl++;} // Veterans Day, 11/11
  if (currentTime("%B") == "November" && d >= 22 && d <= 28 && currentTime("%w") == "4")
    {cout << "Thanksgiving\n"; nEndl++;} // Thanksgiving, fourth Thursday in November
  if (currentTime("%B") == "November" && d >= 22 && d <= 28 && currentTime("%w") == "5")
    {cout << "Black Friday\n"; nEndl++;} // Black Friday, the day after Thanksgiving (fourth Firday in November)
  if (currentTime("%B") == "December" && d == 25)
    {cout << "Christmas\n"; nEndl++;} // Christmas, 12/25
  // Special Days */
  
  Time = currentTime("%I:%M%p"); // e.g. 03:30PM
  for (int i = 5; Time[i]; i++) Time[i] = tolower(Time[i]); // e.g. 03:30pm
  if (Time[0] == '0') Time.erase(0, 1); // e.g. 3:30pm
  getSleeptime();
  int timeIn24 = atoi(currentTime("%H").c_str()); // in 24hr
  if ((timeIn24 >= sleepStart && timeIn24 < sleepEnd)|| (timeIn24 >= sleepStart-24 && timeIn24 < sleepEnd-24)){
	puts("\nSeems like it's time to sleep, Sir.");
	cout << endl;
	if (!confirmation("Do you still want to proceed?", false)) exit(0);
	for (int i = 0; i <= 75; i++) cout << "-"; cout << endl;
	puts("I'll redirect you into playing CFN;\nPermission for access would be denied if you did not win the game.");
	for (int i = 0; i <= 75; i++) cout << "-"; cout << endl;
	system("pause");
	if (!CFN()) {puts("\nGood night"); pause();}
	cout << endl;
	system("pause");
	for(int i = 0; i < 15; i++) cout << endl;
	Time = "!" + Time;
  } // sleeptime
  
  if (existence("tips.txt", "")){
	ifstream fin;
    fin.open("tips.txt");
    string aTip;
    for (int i = 1; i <= random(1, count("tips.txt", "lines")); i++) getline(fin, aTip);
    if (aTip.length() != 0)
      cout << endl << '*' << aTip << '*' << endl << endl;
    fin.close();
  }
  
  cout << currentTime("%A, "); 
  cout << currentTime("%B "); 
  cout << d << currentTime(", 20%y") << endl; // e.g. Thursday, October 23, 2017

  ifstream fin;
  fin.open("agenda.txt");
  const int n = count("agenda.txt", "lines");
  string event[n];
  for (int i = 0; fin.good(); i++) getline(fin, event[i]); // transfer all the content to event[n]
  fin.close();
  
  time_t Date = time(0);
  const int nd = 365, limit = 16, width = 75;
  // nd: maximum number of days; limit: maximum number of lines; width: maximum window's width
  
  for (int d = 0; d < nd; d++, Date += 24*60*60){ // number of days from today
    char date[] = "10/24"; // the date of creating this function
    strftime (date, 6, "%m/%d", localtime(&Date));
	string strDate = date; // string-ize date
    
    bool match = false;
    for (int i = 0; i < n && nEndl < limit; i++){
	  if (keywords(event[i], strDate)) { // !this would fail to detect a date if there is any punctuation before or after it
		match = true;
		if (event[i][0] == '&'){
		  event[i].erase(0, 1);
		  event[i].insert(0, "- "); // change '&' to '- ' if it's in the upcoming days
		}
	    cout << event[i] << endl; nEndl++;
      }
	}
    
	if (match && nEndl <= limit){
      if (d == 0) {
        for (int i = 0; i < width / 5; i++) cout << "TODAY";
      } else {
        for (int i = 0; i < width / 2; i++) {
            if (i % d == 0) {
                cout << d;
            } else {
                cout << "-";
            }
        }
      }
      
      
      cout << endl;
	  nEndl++;
	}
  }
  
  bool match = false;
  for (int i = 0; i < n && nEndl < limit; i++) if (event[i][0] == '&') {cout << event[i] << endl; match = true; nEndl++;}
  if (match){
    for (int i = 0; i <= width; i++) cout << "-"; 
    cout << endl; 
    nEndl++;
  }
}

int main(){
  string Time;
  preamble(Time);
  cout << endl << Time;
  
  string wantToDo;
  puts("\nWhat would you like to do?");
  getline(cin, wantToDo);
  wantToDo[0] = tolower(wantToDo[0]);
  cout << endl;
  if (wantToDo.length() == 1){
    switch (wantToDo[0]){
	  case '?': wantToDo = menu(); break;
      case 'a': wantToDo = "edit the agenda"; break;
      case 'b': wantToDo = "backup all the files to OneDrive"; break;
      case 'd': wantToDo = "write a diary"; break;
      case 'g': wantToDo = "play goBang"; break;
	  case 'h': wantToDo = "view history"; break;
      case 'm': wantToDo = "write a memo"; break;
      case 'o': wantToDo = "open a txt file"; break;
	  case 'p': wantToDo = "print a file"; break;
      case 'r': wantToDo = "open the rules"; break;
      case 't': wantToDo = "edit the tips"; break;
      case 'u': wantToDo = "upload the files"; break;
	  case 'v': wantToDo = "practice vocabulary"; break;
	  case 'w': wantToDo = "wording"; break;
	  case '#': system("cmd"); exit(0);
	}
  }
  else if (wantToDo == "av") wantToDo = "add vocabulary";
  else if (wantToDo == "ea") wantToDo = "event analysis";
  else if (wantToDo == "ed") wantToDo = "encrypt/decipher";
  else if (wantToDo == "ss") wantToDo = "set sleeptime";
  else if (wantToDo == "vm") wantToDo = "view memos";
  else if (wantToDo == "vp") wantToDo = "view pictures";
  else if (wantToDo == "/r" || wantToDo == "-r") wantToDo = "restart";
  else if (wantToDo == "/s" || wantToDo == "-s") wantToDo = "shutdown";
  else if (wantToDo == "help") wantToDo = menu();

  if (wantToDo.length() != 0){
	ofstream fout;
    fout.open("history.txt", ios::app);
	// ifstream fin;
	// fin.open("history.txt");
	
	if (count("history.txt", "lines") > 1 && !existence("history.txt", currentTime("%a. %x"))){ // not for a new history.txt
	  for (int i = 0; i <= 47; i++) fout << "=";
	  fout << endl;
	}

	if (!existence("history.txt", currentTime("%a. %x"))) fout << currentTime("%a. %x");
    fout << endl << Time << ": ";
	fout << wantToDo << endl;	
	fout.close();
	things(wantToDo);
  }
}

string menu(){
  if (!existence("menu.txt", "")) return "[menu does not exit]";
  puts("Menu:");
  const int n = count("menu.txt", "lines");
  string todo[n];
  ifstream fin;
  fin.open("menu.txt");
  for (int i = 0; i < n; i++) {  
	getline(fin, todo[i]);
	todo[i][0] = toupper(todo[i][0]);
    cout << i+1 << ". " << todo[i] << endl;
  }
  
  puts("?. Randomly pick one of the above");
  
  string buf;
  int choice = 0;
  while (choice < 1 || choice > n){
    cout << "\nYour choice: ";
    cin >> buf; choice = atoi(buf.c_str()); cin.ignore(1000, 10);
    todo[choice-1][0] = tolower(todo[choice-1][0]);
	cout << endl;
	if (buf == "?"){
	  choice = random(1, n);
	  cout << endl;
	}
	cout << "Selected: " << todo[choice-1] << endl << endl;
	todo[choice-1][0] = tolower(todo[choice-1][0]);
    if (choice >= 1 && choice <= n) return todo[choice-1];
	else if (choice == 0) exit(0);
    puts("[WRONG COMMAND]");
	cout << "Please try again; enter a number between 1 and " << n << ", or enter 0 to quit." << endl;
  }
}

void things(string wantToDo){ 
  for(int i = 0; wantToDo[i]; i++) wantToDo[i] = tolower(wantToDo[i]);
  
  if (keywords(wantToDo, "upload") || keywords(wantToDo, "backup")) {
    backup();
  } 
  
  else if (wantToDo == "event analysis") {
    eventAnalysis();
  }
  
  else if (keywords(wantToDo, "diary"))
	diary();

  else if (wantToDo == "cmd" || keywords(wantToDo, "command"))
	system("cmd");

  else if (keywords(wantToDo, "restart"))
    system("shutdown -r -t 020");

  else if (keywords(wantToDo, "shutdown"))
    system("shutdown -s -f -t 0");

  else if (keywords(wantToDo, "agenda"))
	system("agenda.txt");
  
  else if (keywords(wantToDo, "tip"))
	system("tips.txt");

  else if (wantToDo == "wording")
	system("wording.txt");

  else if (keywords(wantToDo, "rules"))
	system("rules.txt");
	
  else if (keywords(wantToDo, "sleep") && keywords(wantToDo, "time"))
	setSleeptime();

  else if (wantToDo == "view memos" || wantToDo == "view the memos")
    viewMemos();	  
	
  else if (keywords(wantToDo, "memo"))
	memo();

  else if (keywords(wantToDo, "open") && (keywords(wantToDo, "file") || keywords(wantToDo, "txt")))
    txtOpener();
	
  else if (wantToDo == "history" || wantToDo == "print history" || wantToDo == "view history")
    print("history.txt"); 
	
  else if (keywords(wantToDo, "print"))
    print("fileName");
  
  else if (keywords(wantToDo, "encrypt") || keywords(wantToDo, "decipher"))
	encrypt_decipher(wantToDo);
	
  else if (keywords(wantToDo, "picture"))
	picViewer();

  else if (keywords(wantToDo, "add") && (keywords(wantToDo, "vocab ") || keywords(wantToDo, "words")))
	addVocab();  

  else if (keywords(wantToDo, "vocab")) vocab();
  
  else if (keywords(wantToDo, "gobang"))  goBang();  

  else if (keywords(wantToDo, "color code") || keywords(wantToDo, "colorcode"))
    getColorCode();
  
  else if (wantToDo == "colorshow" || wantToDo == "color show") colorShow();

  else if (wantToDo == "recreate colors.txt") colorsTXT();

  else if (wantToDo == "cfn" && CFN()) // tricky running
    pause();

  else dialogue(wantToDo);
}

void dialogue(string wantToDo){
  if (wantToDo == "sleep") cout << "Good night.";
  else if (wantToDo == "take a nap") cout << "Go head!";
  else if (wantToDo == "read books") cout << "Absolutely do it!";
  else if (wantToDo == "listen to music") cout << "I'd also like to.";
  else if (keywords(wantToDo, "preview")) cout << "Better start now.";
  else if (wantToDo == "guess" || keywords(wantToDo, "?")) cout << "I'm not yet good at answering questions, sorry Sir.";
  else if (keywords(wantToDo, "fuck")) cout << "Always be polite, Sir.";
  else exit(0);
  
  cout << endl;
  pause();
}

void memo(){
  puts("Welcome to Memo.");
  for (int i = 0; i <= 75; i++) cout << "-"; cout << endl;
  string memo;
 
  puts("Start typing below:");
  getline(cin, memo);
  
  ofstream fout;
  fout.open("history.txt", ios::app);
  for (int i = 1; memo.length(); i++){
	memo[0] = toupper(memo[0]);
	fout << "Memo" << i << ": " << memo << endl;
	getline(cin, memo);
  }
  fout.close();
  
  puts("\nDone\n");
  pause();
}

void viewMemos(){
  ifstream fin;
  fin.open("history.txt");
  
  int numberOfMemos;
  while (fin.good()){string aLine; getline(fin, aLine); if (aLine.substr(0, 4) == "Memo") numberOfMemos++;} // counting
  fin.close();
  
  string memo[numberOfMemos];
  fin.open("history.txt");
  for (int i = 0; i < numberOfMemos; i++) {getline(fin, memo[i]); if (memo[i].substr(0, 4) != "Memo") i--;}
  fin.close();
  
  ofstream fout;
  fout.open("allMemos.txt");
  for (int i = numberOfMemos-1; i >= 0; i--){
	memo[i].erase(0, 4); // erase "Memo"
	fout << endl << memo[i];
	if (memo[i][0] == '1' && memo[i][1] == ':') fout << endl;
  }
  fout.close();
  
  numberOfMemos = count("allMemos.txt", "lines");
  string memo2[numberOfMemos];
  fin.open("allMemos.txt");
  for (int i = 0; i < numberOfMemos; i++) getline(fin, memo2[i]);
  fin.close();
  
  for (int i = 0; i < numberOfMemos-1; i++){ // the last line does not need to change because it's blank
	if (memo2[i].length() == 0){
	  int n = 0;
  	  for (int j = i; memo2[j+1].length() != 0; j++) n++; // good
	  if (n == 1) {memo2[i+1].erase(0, 3); memo2[i+1].insert(0, "- ");}
	  else for (int k = 0; k < n/2; k++){ 
		string swap;
		swap = memo2[i+k+1];
		memo2[i+k+1] = memo2[i+n-k];
		memo2[i+n-k] = swap;
	  } // marvelous, just marvelous
	}
  }

  fout.open("allMemos.txt");
  fout << "[NOTE: THIS FILE WILL BE DELETED ONCE CLOSED]";
  for (int i = 0; i < numberOfMemos; i++) fout << endl << memo2[i];
  fout.close();
  
  system("allMemos.txt");
  remove("allMemos.txt");
}

bool CFN(){
  for (int i = 0; i < 10; i++) cout << endl;
  puts("Welcome to CFN108."); // better G127
  for (int i = 0; i <= 75; i++) cout << "-";
  puts("\nEnter a number between 1 and 108:\nIf it's my CFN (Current Favorite Number), then you pass the game;");
  puts("otherwise, you'll know if it needs to be larger or smaller;");
  puts("The maximum number of attempts allowed is 7.");
  cout << endl;
  int theNumber = random(1, 108);
  for (int i = 0; i < 7; i++) cout << endl;
  puts("Let's get started:");
  
  string buf;
  int guess;
  for (int i = 1; i <= 7; i++){
	if (i < 7) cout << "Attempt " << i << ": ";
	else cout << "\nLAST ATTEMPT: ";
	cin >> buf; guess = atoi(buf.c_str()); cin.ignore(1000, 10);
	
    if (guess == t('m') * 99) {puts("\n\n    Welcome, Master.\n"); return true;} // a way to bypass CFN by looking at time
	else if (guess <= 0 || guess > 108) {puts("Please abide by the rules stated above:)\n"); i--; continue;}
	else if (i != 7 && guess > theNumber) puts("Wrong; try a smaller number.");
	else if (i != 7 && guess < theNumber) puts("Wrong; try a larger number.");
	else {
	  if (guess != theNumber) continue;
	  cout << endl;
	  if (i == 1) puts("Are you a hacker?!\nYou've got my CFN at the first attempt!");
	  else if (i >= 2 && i <= 4) puts("Fabulous work!");
	  else if (i == 7) cout << "Congrats, pal.\nBut what takes you so long to figure out that " << theNumber << " is my favorite number?\n";
	  else puts("Finally, You've found it.");
	  return true;
	}
  }
  
  int i = random(1, 3);
  cout << "\n[Wrong again]\n";
  cout << endl;
  if (i == 1) 
	puts("Theoretically, there is 100 percent of chance\nfor you to get my CFN by now, but you failed,\nwhich really breaks my heart:(");
  else if (i == 2) 
	puts("Frankly, what you've just done\nis like being wrong at the fourth attempt trying to\nsolve a multiple-choice question that has four choices:)");
  else puts("I hate you, bye-bye.");
  return false;
}

void diary() {
  if (existence("diary.txt", currentTime("%x"))) {system("diary.txt"); exit(0);} 
  
  string buf;
  
  string event[16];
  cout << endl;
  cout << "What happened today?\n";
  cout << "1" << ": ";
  getline(cin, event[0]); // the first event
  
  for (int i = 1; i < 16 && event[i-1].length() != 0; i++){ 
  // if the previous event is not empty, record the next event
    event[i-1][0] = toupper(event[i-1][0]);
    cout << i+1 << ": ";
    getline(cin, event[i]);
  }
  
  string keyword;
  cout << endl;
  cout << "One word to summarize today: ";
  cin >> keyword; cin.ignore(1000, 10);
  keyword[0] = toupper(keyword[0]);
  
  int evaluation = -1;
  cout << endl;
  cout << "Score for today: ";
  cin >> buf; cin.ignore(1000, 10);
  evaluation = atoi(buf.c_str());
  validate(evaluation, -100, 100);
  
  ofstream fout;
  fout.open("diary.txt", ios::app);
  fout << currentTime("%a. %x") << endl;
  
  fout << '#' << keyword << endl;
 
  fout << "\nEvents: \n";
  for (int i = 0; i < 16 && event[i].length() != 0; i++) fout << i+1 << ": " << event[i] << endl;
  fout << endl;
  
  fout << "Score for today: " << evaluation << endl << endl;
  
  for (int i = 0; i <= 75; i++) fout << "-";
  fout << endl;
  fout.close();
  
  cout << "\n\nDone\n\n";
  pause();
}

void txtOpener(){
  const int nTXTs = 10; 
  string allTXTs[nTXTs] = {"agenda", "colors", "diary", "events", "history", "menu", "rules", "tips", "vocabulary", "wording"};
  for (int i = 0; i < nTXTs; i++){
	  allTXTs[i] += ".txt";
	  cout << allTXTs[i][0] << ". " << "Open " << allTXTs[i] << endl;
  }
  
  char choice;
  cout << "\nYou wanna open: ";
  cin >> choice; cin.ignore(1000, 10);

  for (int i = 0; i < nTXTs; i++){
    if (choice == allTXTs[i][0]){
	  system(allTXTs[i].c_str());
	  exit(0);
	}
  }
  
  // if none matches
  puts("\nWRONG COMMAND");
  pause();
}

void picViewer(){
  puts("Welcome to picViewer.");
  for (int i = 0; i <= 75; i++) cout << "-";
  cout << endl;
  
  const int n = 7;
  string picture[n] = {"libraryHours.png", "mapOfChina.jpg", "orderTE.png", "orderWSJ.png", "strftime.png", "USmap.jpg", "worldMap.jpg"};
  
  for (int i = 0; i < n; i++) cout << i+1 << ". " << picture[i] << endl;
  puts("\nWhich of the above would you like to view?");
  
  int choice = 0;
  cin >> choice; cin.ignore(1000, 10);
  
  picture[choice-1] = "pictures\\" + picture[choice-1];
  system(picture[choice-1].c_str());
  
  //char response = 'N';
  //puts("Wanna view another pic? [Y/N]");
  //cin >> response; cin.ignore(1000, 10);
  //if (toupper(response) == 'Y') picViewer();
  //What's wrong with the last four lines?
}

void vocab(){
  for (int l = 0; l < 10; l++) cout << endl;
  puts("\nWelcome to vocab.");
  for (int i = 0; i <= 75; i++) cout << "-";
  cout << endl;
  if (!existence("vocabulary.txt", "")){
    ofstream fout;
    fout.open("vocabulary.txt", ios::app);
    fout.close();
  }

  puts("One word will be shown on screen each time:");
  puts("a. For what you are not familiar with, press 'N';");
  puts("b. You may choose to exit at any time by entering a 'Q';");
  puts("c. Else, enter any letter to continue;");
  puts("d. You'll be prompted to exit once the number of N's reaches 9.\n");
  
  for (int l = 0; l < 7; l++) cout << endl;
  system("pause");
  
  int numberOfWords = count("vocabulary.txt", "words");
  ifstream fin;
  fin.open("vocabulary.txt");
  string word[numberOfWords];
  for (int i = 0; i < numberOfWords; i++) fin >> word[i];
  fin.close();
  
  for (int i = 0; i < numberOfWords; i++) 
	for (int j = 0; word[i][j]; j++)
     if (word[i][j] == '_') word[i][j] = ' ';
  
  int i, count = 1, N = 0;
  string dunno[numberOfWords];
  char response;
  
  fin.open("vocabulary.txt");
  while (fin.good()){
	if (numberOfWords == 0) break;
	i = random(0, numberOfWords);
	cout << count << ") " << word[i] << endl;
	cin >> response; cin.ignore(1000, 10);
	if (toupper(response) == 'Q') break;
	else if (toupper(response) == 'N') {dunno[N] = word[i]; N++;}
	if (N == 9) break;
	count++;
	for (int j = i; j < numberOfWords-1; j++) word[j] = word[j+1];
	numberOfWords--; // delete the word showed
  }
  fin.close();
  
  if (N == 0 && count >= 20) puts("\n\nExcellent!");
  else if (N != 0){
    cout << endl << "Up to now, there were " << N << " words that you didn't know, as shown below:\n";
	for (int j = 0; j < N; j++) cout << j+1 << ": " << dunno[j] << endl;
	
	ofstream fout;
	fout.open("history.txt", ios::app);
	fout << "{";
	for (int j = 0; j < N; j++) {
	  if (j == N-1) fout << dunno[j] << "}" << endl;
	  else fout << dunno[j] << "; ";
	}
	fout.close();
  }
  pause();
}

void addVocab(){
  for (int i = 0; i < 9; i++) cout << endl;
  string aWord;
  puts("Guide:");
  puts("- Enter one word each time");
  puts("- Never enter a blank space--use _ instead");
  puts("- Enter 'Q' as the last word to exit");
  for (int i = 0; i <= 75; i++) cout << "-";
  for (int i = 0; i < 10; i++) cout << endl;
  system("pause");
  ofstream fout; fout.open("vocabulary.txt", ios::app);
  for (int i = 0; aWord != "q" && aWord != "Q"; i++){
	cout << i+1 << ": ";
    cin >> aWord; cin.ignore(1000, 10);
    if (aWord == "q" || aWord == "Q") break;
    else if (existence("vocabulary.txt", aWord)){
	  cout << "[The word \"" << aWord << "\" already exists in vocabulary.txt]\n";
	  puts("Please enter another word");
      i--;
	  continue;
	}
	else fout << " " << aWord;
  }
  fout.close();
  pause();
}

void encrypt_decipher(string wantToDo){
  int numberOfLines = 0;
  ofstream fout;
  fout.open("temporary.txt");
  fout << "[Start pasting or typing your text after pressing Ctrl-A.\nWhen done, save and close the file.]";
  fout.close();
  
  system("temporary.txt");
  ifstream fin;
  fin.open("temporary.txt");  
  while (fin.good()) {string lines; getline(fin, lines); numberOfLines++;}
  fin.close();
  fin.open("temporary.txt");
  string* content = new string[numberOfLines];
  for (int i = 0; i < numberOfLines; i++) getline(fin, content[i]);
  fin.close(); 
  
  if (wantToDo == "decipher" || wantToDo == "deciphering"){
    fout.open("temporary.txt");
	fout << "[WARNING: this file is going to be deleted once closed.\n";
	fout << "Below is the deciphered content:]\n\n";
    for (int i = 0; i < numberOfLines - 1; i++) fout << decipher(content[i]) << endl;
    fout << decipher(content[numberOfLines - 1]); // avoid endl
    fout.close();
	system("temporary.txt");
  }
  
  else {
	fout.open("temporary.txt");
	fout << "[WARNING: this file is going to be deleted once closed.\n";
	fout << "Below is the encrypted content:]\n\n";
    for (int i = 0; i < numberOfLines - 1; i++) fout << encrypt(content[i]) << endl;
    fout << encrypt(content[numberOfLines - 1]);
    fout.close();
	system("temporary.txt");
  }
  remove("temporary.txt");

  if (confirmation("Wanna do for another text?", false)){
	char response;
	cout << "Encrypt/Decipher? [E/D]\n";
	cin >> response; cin.ignore(1000, 10);
	if (toupper(response) == 'E') encrypt_decipher("encrypt");
	else if (toupper(response) == 'D') encrypt_decipher("decipher");
  }
}

string encrypt(string content){
  for (int i = 0; content[i]; i++){
    switch (content[i]){
	  case 'a': {content[i] = '#'; break;} 
	  case 'b': {content[i] = 'e'; break;}
	  case 'c': {content[i] = 'f'; break;}
	  case 'd': {content[i] = 'g'; break;}
	  case 'e': {content[i] = '!'; break;} 
	  case 'f': {content[i] = 'i'; break;}
	  case 'g': {content[i] = 'j'; break;}
	  case 'h': {content[i] = 'k'; break;}
	  case 'i': {content[i] = '@'; break;} 
	  case 'j': {content[i] = 'm'; break;}
	  case 'k': {content[i] = 'n'; break;}
	  case 'l': {content[i] = 'o'; break;} 
	  case 'm': {content[i] = 'p'; break;} 
	  case 'n': {content[i] = '2'; break;} 
	  case 'o': {content[i] = 'r'; break;}
	  case 'p': {content[i] = 's'; break;}
	  case 'q': {content[i] = 't'; break;}
	  case 'r': {content[i] = 'u'; break;}
	  case 's': {content[i] = ' '; break;} 
	  case 't': {content[i] = '9'; break;} 
	  case 'u': {content[i] = 'x'; break;}
	  case 'v': {content[i] = 'y'; break;}
	  case 'w': {content[i] = 'z'; break;}
	  case 'x': {content[i] = 'a'; break;}
	  case 'y': {content[i] = 'b'; break;}
	  case 'z': {content[i] = 'c'; break;}
	  case '1': {content[i] = '-'; break;} 
	  case '2': {content[i] = '4'; break;}
	  case '3': {content[i] = ':'; break;}
	  case '4': {content[i] = '6'; break;}
	  case '5': {content[i] = '7'; break;}
	  case '6': {content[i] = '8'; break;}
	  case '7': {content[i] = 'w'; break;}
	  case '8': {content[i] = '0'; break;}
	  case '9': {content[i] = '1'; break;}
	  case '0': {content[i] = 'q'; break;}
	  case '"': {content[i] = '.'; break;}
	  case ' ': {content[i] = ','; break;}
	  case ',': {content[i] = '3'; break;}
	  case '.': {content[i] = '5'; break;}
	  case '-': {content[i] = '"'; break;}
	  case ':': {content[i] = 'v'; break;}
	  case '#': {content[i] = 'd'; break;}
	  case '@': {content[i] = 'l'; break;}
	  case '!': {content[i] = 'h'; break;}
	}
  }
  return content;
}

string decipher(string content){
  for (int i = 0; content[i]; i++){
    switch (content[i]){
	  case '#': {content[i] = 'a'; break;}
	  case 'e': {content[i] = 'b'; break;}
	  case 'f': {content[i] = 'c'; break;}
	  case 'g': {content[i] = 'd'; break;}
	  case '!': {content[i] = 'e'; break;}
	  case 'i': {content[i] = 'f'; break;}
	  case 'j': {content[i] = 'g'; break;}
	  case 'k': {content[i] = 'h'; break;}
	  case '@': {content[i] = 'i'; break;}
	  case 'm': {content[i] = 'j'; break;}
	  case 'n': {content[i] = 'k'; break;}
	  case 'o': {content[i] = 'l'; break;}
	  case 'p': {content[i] = 'm'; break;}
	  case '2': {content[i] = 'n'; break;}
	  case 'r': {content[i] = 'o'; break;}
	  case 's': {content[i] = 'p'; break;}
	  case 't': {content[i] = 'q'; break;}
	  case 'u': {content[i] = 'r'; break;}
	  case ' ': {content[i] = 's'; break;}
	  case '9': {content[i] = 't'; break;}
	  case 'x': {content[i] = 'u'; break;}
	  case 'y': {content[i] = 'v'; break;}
	  case 'z': {content[i] = 'w'; break;}
	  case 'a': {content[i] = 'x'; break;}
	  case 'b': {content[i] = 'y'; break;}
	  case 'c': {content[i] = 'z'; break;}
	  case '-': {content[i] = '1'; break;}
	  case '4': {content[i] = '2'; break;}
	  case ':': {content[i] = '3'; break;}
	  case '6': {content[i] = '4'; break;}
	  case '7': {content[i] = '5'; break;}
	  case '8': {content[i] = '6'; break;}
	  case 'w': {content[i] = '7'; break;}
	  case '0': {content[i] = '8'; break;}
	  case '1': {content[i] = '9'; break;}
	  case 'q': {content[i] = '0'; break;}
	  case '.': {content[i] = '"'; break;}
	  case ',': {content[i] = ' '; break;}
	  case '3': {content[i] = ','; break;}
	  case '5': {content[i] = '.'; break;}
	  case '"': {content[i] = '-'; break;}
	  case 'v': {content[i] = ':'; break;}
	  case 'd': {content[i] = '#'; break;}
	  case 'l': {content[i] = '@'; break;}
	  case 'h': {content[i] = '!'; break;}
	}
  }
  return content;
}

void setSleeptime(){
  if (existence("history.txt", "&Sleeptime") == true){
	if (sleepStart > 24) cout << "[The current sleeptime is from " << sleepStart-24 << "am to the next day " << sleepEnd-24 << "am]\n";
	else cout << "[The current sleeptime is from " << sleepStart-12 << "pm to " << sleepEnd-24 << "am]\n";
	if (!confirmation("Wanna change it?", false)) exit(0);
  } // show the current setting

  cout << endl;
  string buf;
  int hours, getup;
  cout << "Enter the number of hours you want for sleep [from 2 to 15]: ";
  cin >> buf; hours = atoi(buf.c_str());
  cin.ignore(1000, 10);
  
  if (hours < 2 || hours > 15){
    puts("[WRONG INPUT]");
	puts("Sleep hours default to 8");
	hours = 8;
  }
  
  cout << "Enter the time (in o'clock am) you want for getup [from 3 to 11]: ";
  cin >> buf; getup = atoi(buf.c_str());
  cin.ignore(1000, 10);
  
  if (getup < 3 || getup > 11){
    puts("[WRONG INPUT]");
	puts("Time for getup default to 6am");
	getup = 6;
  }
  
  int sleep = getup - hours;
  
  ofstream fout;
  fout.open("history.txt", ios::app);
  if (sleep < 0) {
	sleep += 12;
	fout << "&Sleeptime set from " << sleep << "pm to " << getup << "am.\n";
	cout << "Sleeptime is set, which is from " << sleep << "pm to the next day " << getup << "am.\n";
  }
  else {
	fout << "&Sleeptime set from " << sleep << "am to " << getup << "am.\n";
	cout << "Sleeptime is set, which is from " << sleep << "am to " << getup << "am.\n";
  }
  fout.close();
  pause();
}

void print(string fileName){
  const int out = 66;
  if (fileName != "fileName"){
	const int nLines = count(fileName, "lines");
    ifstream fin;
    fin.open(fileName.c_str());
    string lines[nLines];
    for (int i = 0; fin.good(); i++) getline(fin, lines[i]);
    fin.close();

    for (int i = 0; i < 9; i++) cout << endl; // formatting
	for (int i = 0; i <= 75; i++) cout << "-"; // formatting
    cout << endl; // formatting 
	
	int start = 0;
	if (nLines > out) {start = nLines - out; cout << "...\n";}// essential; or 'start' might be negative
    for (int i = start; i < nLines; i++) cout << lines[i] << endl; // the only place in the function to really print a file
	
	for (int i = 0; i <= 75; i++) cout << "-"; // formatting
    cout << endl; // formatting 
	if (nLines > out) cout << "The last " << out << " lines of " << fileName << " is as shown above\n";
	else cout << "The content of " << fileName << " is as shown above\n";
    
	cout << endl;
    if (!confirmation("Wanna print another file?", false)) exit(0); // call confirmation
}
  
  cout << endl;
  const int nFiles = 6;
  string allFiles[nFiles] = {"agenda", "diary", "history", "menu", "rules", "tips"};
  for (int i = 0; i != nFiles; i++) allFiles[i] += ".txt";
  puts("Which one of the following would you like to print?");
  for (int i = 0; i < nFiles; i++) cout << i+1 << ". " << allFiles[i] << endl;
  cout << endl << "Your choice: ";
	 
  int choice;
  string buf;
  cin >> buf; choice = atoi(buf.c_str()); cin.ignore(1000, 10);
	  
  while (choice < 1 || choice > nFiles){
  puts("[WRONG COMMAND]");
  cout << "Please try again or enter 'q' to exit: ";
  cin >> buf; choice = atoi(buf.c_str()); cin.ignore(1000, 10);
  if (buf.length() == 1 && tolower(buf[0]) == 'q') exit(0); 
  else print(allFiles[choice-1]);
  }
	 
  print(allFiles[choice-1]);
}
// //   / /        / / / / /   / / // 
void backup() {
    system("robocopy C:\\Users\\user\\Desktop\\Programs\\Thod \
        C:\\Users\\user\\OneDrive\\Notes\\ThodFiles \
        diary.txt agenda.txt rules.txt tips.txt wording.txt");
    system("xcopy * C:\\Users\\user\\OneDrive\\Notes\\ThodFiles\\Backup \
        /S /Q /EXCLUDE:noBackup.txt");
    pause(); // backup all Thod's files except the files specified in noBackup.txt
}
  
void eventAnalysis() {
    cout << endl;
    cout << "order: ";
    system("javac EventAnalysis.java");
      
    string order;
    getline(cin, order);
    string str = "java EventAnalysis " + order;
      
    const char* response = str.c_str();
    cout << endl;
    system(response);
    cout << endl;
    
    // continue?
    cout << endl;
    confirmation("Wanna see in another order?", false) ? eventAnalysis() : pause();
}
