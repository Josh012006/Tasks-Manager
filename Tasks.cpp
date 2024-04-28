//Instructions au préprocesseur
#include <iostream>
#include <string>
#include <cstdio>
#include <fstream>
#include <set>

//Inclusion de la namespace std
using namespace std;

//Déclaration de fonctions
std::fstream opening(const std::string& filename, char choice);
void reading(std::fstream& file);
void writing(std::fstream& file, std::string thing);
void erasing(std::fstream& file, std::string name, std::string target);


//La fonction d'ouverture d'un fichier
std::fstream opening(const std::string& filename, char choice)
{
    //Ouverture en écriture
    if(choice == 'w')
    {
        std::fstream file(filename, std::ios::out);
        if(!file.is_open())
        {
            std::cerr << "Erreur : Impossible d'ouvrir le fichier " << filename << std::endl;
            return std::fstream();
        }

        return file;
    }

    //Ouverture en lecture
    else if (choice == 'r')
    {
        std::fstream file(filename, std::ios::in);
        if(!file.is_open())
        {
            std::cerr << "Erreur : Impossible d'ouvrir le fichier " << filename << std::endl;
            return std::fstream();
        }

        return file;
    }

    //Ouverture en append ou en ajout
    else if(choice == 'a')
    {
        std::fstream file(filename, std::ios::app);
        if(!file.is_open())
        {
            std::cerr << "Erreur : Impossible d'ouvrir le fichier " << filename << std::endl;
            return std::fstream();
        }

        return file;
    }

    //Ouverture avec toutes les permissions
    else if(choice == 't')
    {
        std::fstream file(filename, std::ios::in | std::ios::out | std::ios::app);
        if(!file.is_open())
        {
            std::cerr << "Erreur : Impossible d'ouvrir le fichier " << filename << std::endl;
            return std::fstream();
        }

        return file;
    }


    //Au cas d'une mauvaise entrée de l'utilisateur
    else
    {
        std::cerr << "Deuxième paramètre incorrect!" << std::endl;
        std::cerr << "Erreur : Impossible d'ouvrir le fichier " << filename << std::endl;
        return std::fstream();
        
    }
}


//Fonction pour lire le contenu du fichier
void reading(std::fstream& file)
{
    /*On utilisera ici un ensemble de string dans lequel on transfère tout le contenu du fichier */
    set<string> list;

    string text;
    while(getline(file, text))
    {
        list.insert(text);  
    }

    /*On vérifiera ensuite que l'ensemble (et donc le fichier) n'est pas vide avant de faire sortir sur la sortie standard l'ensemble du contenu*/
    int n = list.size();
    if (n == 0)
        std::cout << "No task in the list." << std::endl;
    else
    {
        std::cout << "Here is your list" << std::endl;
        for(string b : list)
        {
            cout << "- " << b << endl;
        }
    }
}


//Fonction simple pour écrire dans le fichier et gérer les erreurs d'écriture
void writing(std::fstream& file, string thing)
{
    file << thing << std::endl;
    if (!file.good()) 
    {
        std::cerr << "Erreur : Impossible d'écrire dans le fichier." << std::endl;
    }
}


//Fonction pour effacer une des tâches
void erasing(std::fstream& file, std::string name, std::string target)
{
    //On crée une nouvelle fois un ensemble dans lequel on transfère les tâches
    set<string> list;

    string text;
    while(getline(file, text))
    {
        list.insert(text);  
    }

    //On vérifie que la tâche à supprimer est effectivement dans l'ensemble
    bool verify = list.count(target);
    if(verify == 0)
        std::cout << "No such task in the list." << std::endl;
    else
    {
        //On efface la tâche
        list.erase(target);
        std::cout << "Task deleted." << std::endl;
    }

    //On s'assure de fermer le fichier
    file.close();

    //On le réouvre mais cette fois-ci en mode écriture uniquement. Cela efface le contenu du fichier
    file = opening(name, 'w');

    //On ajoute maintenant chacune des tâches restantes dans l'ensemble après suppression
    for(string b : list)
    {
        writing(file, b);
    }

    //On referme le fichier
    file.close(); 
    //On le réouvre avec toutes les permissions pour ne pas bloquer la suite du programme notamment l'ajout simple de tâches, par mégarde  
    file = opening(name, 't');
    
}


//Fonction principale
int main (void)
{
    //Variables utiles
    int request;
    bool here = true;

    const string name = "Tasks.txt";

    //On ouvre le fichier avec toutes les permissions
    fstream MyFile = opening(name, 't');

    //On dit à l'utilisateur ce qu'il peut faire
    std::cout << "You can: " << std::endl;
    std::cout << "1. Add a task." << std::endl;
    std::cout << "2. Delete a task."<< std::endl;
    std::cout << "3. List my tasks." << std::endl;
    std::cout << "4. Finished."<< std::endl;
    std::cout << "Choose among those numbers."<< std::endl;

    while(here == true)
    {
        //A ce niveau, on demande à l'utilisateur qu'est ce qu'il veut faire exactement
        std::cout<< "What do you want to do ?" << std::endl;
        std::cin >> request; std::cin.ignore();

        //Grâce à cette switch, on vérifie qu'il a vrm entré un nombre entre1, 2, 3 et 4
        switch (request)
        {
            case 1:
            { 

                //On recueille les tâches de l'utilisateur
                string a;
                std::cout << "Print below the task you want to add."<< std::endl;
                std::getline(std::cin, a);
                
                //On utilise la fonction writing pour enrégistrer les tâches dans le fichier
                writing(MyFile, a);

                std::cout << "Task added." << std::endl;


                break;
            }

            case 2:
            {

                //On recueille de l'utilisateur la tâche qu'il veut enlever de sa liste
                string forget;
                std::cout << "Print below the task you want to delete."<< std::endl;
                std::getline(std::cin, forget);

                //On laisse la fonction erasing gérer la suppression
                erasing(MyFile, name, forget);


                break;
            }

            case 3:
            {
                //On s'assure de fermer le fichier et de le réouvrir en mode lecture seulement
                MyFile.close();
                MyFile = opening(name, 'r');

                //On utilise la fonction reading pour faire sortir les tâches sous forme de liste
                reading(MyFile);

                // On ferme à nouveau le fichier et on le réouvre avec toutes les permissions pour ne pas bloquer la suite du programme notamment l'ajout simple de tâches, par mégarde 
                MyFile.close();
                MyFile = opening(name, 't');


                break;
            }
            
            case 4:

                //On met fin au programme 
                here = false;


                break;

            default:

                //On lui notifie une mauvaise entrée et on lui demande de recommencer
                std::cout << "Le numéro entré n'est pas valide. Si vous avez fini veuillez entrer 4." << std::endl;

                
                break;
        }

    }

    //On s'assure de fermer le fichier après utilisation
    MyFile.close();

    //Fin du programme
    return 0;


    //NB: Notons ici qu'on aurait pu faire tout le travail avec uniquement un élément set et sans un fichier
    //Le fichier permet de faire en sorte que les tâches de l'utilisateur soient stockées de façon réelles sur son pc
    //et empêche ainsi de les perdre une fois le programme terminé.
}