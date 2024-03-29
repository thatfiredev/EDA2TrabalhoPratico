#include <iostream>
#include "CityList.h"
#include "StreetList.h"
#include "MatchQueue.h"
#include "RoundList.h"
#include "RouteList.h"
#include "BridgeList.h"
#include <ctime>
#include <fstream>

using namespace std;

int readClue()
{
    char line[256];
    ifstream is("Pista.txt");
    int number = 0;
    if(is)
    {
        while(is.getline(line, sizeof(line)))
            number = atoi(line);
        is.close();
    }
    else{
        cout << "FICHEIRO 'Pista.txt' NÃO ENCONTRADO!";
    }
    return number;
}

tm* addDays( struct tm* date, int days )
{
    const time_t ONE_DAY = 24 * 60 * 60;
    time_t date_seconds = mktime( date ) + (days * ONE_DAY);
    *date = *localtime( &date_seconds );
    return date;
}

tm* getNextSaturday(tm *gmnow)
{
    while(gmnow->tm_wday != 6)
        addDays(gmnow, 1);
    return gmnow;
}

int whoDidTheyPlayAgainst(int team, RoundList::Round *round)
{
    MatchQueue::Match *match = round->matchQueue->head;
    while(match!=NULL)
    {
        if(team == match->teamHome)
            return match->teamAway;
        if(team == match->teamAway)
            return match->teamHome;
        match = match->next;
    }
    return -1;
}

int indexOf(int n, vector <int> v){
    for(int i=0; i< v.size();i++)
        if(v[i] == n)
            return i;
    return -1;
}

void listenForInput(CityList *cityList, RouteList *routeList)
{
    cout << "Introduza um comando:";
    string command;
    cin >> command;
    if(command == "SAIR")
    {
        cout << "GAME OVER :(" << endl;
        return;
    } else if( command == "CRIAR_MAPA_DO_JOGO_CIDADE")
    {
        //cout << "ID_CIDADE:";
        int idCity;
        cin >> idCity;
        //cout << "NOME_CIDADE:";
        string nameCity;
        cin >> nameCity;
        CityList::City *city = new CityList::City(idCity, nameCity);
        cityList->add(city);
        cout << "SUCESSO! CIDADE ADICIONADA {ID: "<<idCity <<
                "; NOME: "<<nameCity<<"}!" << endl;
    }else if(command == "CRIAR_MAPA_DO_JOGO_ESTRADA")
    {
        //cout << "ID_CIDADE_ORIGEM:";
        int city1;
        cin >> city1;
        //cout << "ID_CIDADE_DESTINO:";
        int city2;
        cin >> city2;

        //cout << "DISTANCIA:";
        double distance;
        cin >> distance;

        CityList::City* city = cityList->getById(city1);
        CityList::City* destination = cityList->getById(city2);
        StreetList::Street *street = new StreetList::Street(destination, distance, 0,0,0);
        if(city1 == city2)
            cout << "ERRO! O MAPA_DO_JOGO NÃO PODE CONTER ESTRADAS INTERNAS NUMA"
                    " CIDADE! REVEJA O CONCEITO DE PONTES E CASAS NUMA CIDADE SE"
                    " QUER EXPANDIR UMA CIDADE!" << endl;
        else if(city == NULL)
            cout << "ERRO! NÃO EXISTE NENHUMA CIDADE COM ID=" << city1 << endl;
        else if(destination == NULL)
            cout << "ERRO! NÃO EXISTE NENHUMA CIDADE COM ID=" << city2 << endl;
        else if(cityList->size() %2 != 0)
            cout << "ERRO! O MAPA_DO_JOGO CONTEM UM NUMERO IMPAR DE CIDADES!" << endl;
        else
        {
            if(city->streetList == NULL)
                city->streetList = new StreetList;
            city->streetList->add(street);
            cout << "SUCESSO! ESTRADA ADICIONADA {ORIGEM:"<<city1 <<
                 "; DESTINO:"<<city2 << "; DISTANCIA=" <<distance<<"KM}!" << endl;
        }
    }else if(command == "ACTUALIZAR_MAPA_DO_JOGO_INFO_CRIMINOSA")
    {
        //cout << "ID_CIDADE_ORIGEM:";
        int city1;
        cin >> city1;
        //cout << "ID_CIDADE_DESTINO:";
        int city2;
        cin >> city2;

        //cout << "NUMERO_DE_LADROES_NESTA_ESTRADA:";
        int thieves;
        cin >> thieves;
        //cout << "NUMERO_DE_POLICIAS_NESTA_ESTRADA:";
        int police;
        cin >> police;

        CityList::City *origin = cityList->getById(city1);
        if(city1 == city2)
        {
            cout << "ERRO! NÃO EXISTE ESTRADA DE " << city1 << " PARA " << city1 << endl;
        }
        else if(thieves < 0)
        {
            cout << "ERRO! NÃO PODE EXISTIR UM NÚMERO NEGATIVO DE LADRÕES" << endl;
        }
        else if(police < 0)
        {
            cout << "ERRO! NÃO PODE EXISTIR UM NÚMERO NEGATIVO DE POLÍCIAS" << endl;
        }
        else if(origin == NULL)
        {
            cout << "ERRO! NÃO EXISTE NENHUMA CIDADE COM ID=" << city1 << endl;
        }
        else
        {
            origin->streetList->printStreet(city1, city2);
            //cout << "ID_ESTRADA:" << endl;
            int streetId;
            cin >> streetId;
            if(origin->streetList->getById(streetId) == NULL)
                cout << "ERRO! NÃO EXISTE NENHUMA ESTRADA COM ID="<<streetId<<"!" << endl;
            else
                origin->streetList->updateStreet(streetId, thieves, police);
        }
    }else if(command == "ACTUALIZAR_MAPA_DO_JOGO_INFO_CASAS")
    {
        //cout << "ID_CIDADE:";
        int city;
        cin >> city;
        //cout << "NUMERO_DE_CASAS:";
        int housesNr;
        cin >> housesNr;
        if(housesNr < 0)
            cout << "ERRO! UMA CIDADE NÃO PODE CONTER UM NÚMERO NEGATIVO DE CASAS" << endl;
        else if(cityList->isEmpty())
        {
            cout << "ERRO! ADICIONE CIDADES ANTES DE ADICIONAR CASAS!" << endl;
        }else
            cityList->addHouses(city, housesNr);
    }else if(command == "CRIAR_CIDADE_PONTE")
    {
        //cout << "ID_CASA_ORIGEM:";
        int house1;
        cin >> house1;
        //cout << "ID_CASA_DESTINO:";
        int house2;
        cin >> house2;

        //cout << "DISTANCIA:";
        double distance;
        cin >> distance;

        if(house1 == house2){
            cout << "ERRO! NÃO É POSSÍVEL ADICIONAR UMA PONTE ENTRE UMA CASA E ELA MESMA!"<<endl;
        }
        else if(cityList->isEmpty()){
            cout << "ERRO! ADICIONE CIDADES ANTES DE ADICIONAR PONTES!" << endl;
        }else {
            cityList->printList();
            int cityId;
            //cout << "ID_CIDADE:";
            cin >> cityId;
            CityList::City *city = cityList->getById(cityId);
            if(city==NULL)
            {
                cout << "ERRO! NÃO EXISTE NENHUMA CIDADE COM ID="<<cityId << endl;
            }
            else
            {
                HouseList::House *firstHouse = city->houseList->getById(house1);
                if(firstHouse == NULL)
                    cout << "ERRO! NÃO EXISTE NENHUMA CASA COM ID="<<house1 << endl;
                else{
                    HouseList::House *secondHouse = city->houseList->getById(house2);
                    if(secondHouse == NULL)
                    {
                        cout << "ERRO! NÃO EXISTE NENHUMA CASA COM ID="<<house1 << endl;
                    } else{
                        BridgeList::Bridge *bridge =
                                new BridgeList::Bridge(distance, secondHouse);
                        if(firstHouse->bridgeList == NULL)
                            firstHouse->bridgeList = new BridgeList;
                        firstHouse->bridgeList->add(bridge);
                        cout << "SUCESSO! PONTE ADICIONADA {ORIGEM:"<<house1 <<
                             "; DESTINO:"<<house2 << "; DISTANCIA=" <<distance<<"}!" << endl;
                    }
                }

            }
        }
    }else if(command == "ADICIONAR_ROTA")
    {
        //cout << "ID_CIDADE_ORIGEM:";
        int city1;
        cin >> city1;
        //cout << "ID_CIDADE_DESTINO:";
        int city2;
        cin >> city2;

        CityList::City *origin = cityList->getById(city1);
        if(city1 == city2)
        {
            cout << "ERRO! NÃO É POSSÍVEL CRIAR ROTA DE UMA CIDADE PARA ELA MESMA." << endl;
        } else if(origin == NULL)
        {
            cout << "ERRO! NÃO EXISTE NENHUMA CIDADE COM ID=" << city1 << endl;
        } else{
            CityList::City *destination = cityList->getById(city2);
            if(destination == NULL)
            {
                cout << "ERRO! NÃO EXISTE NENHUMA CIDADE COM ID=" << city2 << endl;
            } else{
                cityList->printList();
                int stopoverId;
                double distance;
                CityList::City *stopCity;
                RouteList::Route *route = new RouteList::Route(city1, city2);
                StopoverQueue::Stopover *stopover;
                do{
                    cout <<"(Introduza 0 quando terminar) PARAGEM_ID:";
                    cin >> stopoverId;
                    if(stopoverId == 0)
                        break;
                    cout <<"DISTANCIA_ENTRE_A_ORIGEM_E_A_PARAGEM:";
                    cin >> distance;
                    stopCity = cityList->getById(stopoverId);
                    if(stopoverId == city1 || stopoverId == city2 || stopCity == NULL)
                    {
                        cout << "PARAGEM INVÁLIDA" << endl;
                    }
                    else{
                        stopover = new StopoverQueue::Stopover(stopoverId, distance);
                        route->stopoverQueue->enqueue(stopover);
                    }
                }while (stopoverId != 0);
                RouteList::Route* aux = routeList->getRoute(route);
                if(aux == NULL)
                {
                    routeList->add(route);
                    cout << "SUCESSO! ROTA ADICIONADA." << endl;
                    routeList->printRoute(route);
                }
                else{
                    cout << "ERRO! JÁ EXISTE UMA ROTA SEMELHANTE:" << endl;
                    routeList->printRoute(aux);
                }
            }
        }

    }else if(command == "GERAR_CALENDARIO_DE_FUTEBOL")
    {
        int size = cityList->size();
        if(size %2 != 0)
            cout << "ERRO! O MAPA_DO_JOGO CONTEM UM NUMERO IMPAR DE CIDADES!" << endl;
        else{
            vector<int> cityIds;
            vector<bool> hasPlayedRound;
            CityList::City *city = cityList->head;
            while(city!=NULL)
            {
                cityIds.push_back(city->id);
                hasPlayedRound.push_back(false);
                city = city->next;
            }

            time_t now = time(0);
            tm* gmnow = gmtime(&now);
            tm* date = getNextSaturday(gmnow);

            int roundSize =cityList->size()-1;
            int optionalTeam = cityIds[size-1];
            cityIds[roundSize] = optionalTeam;
            RoundList* roundList = new RoundList(cityList->size());
            MatchQueue::Match* match2;
            RoundList::Round *currentRound = roundList->head;

            //First round
            int j = roundSize;
            int scheduledGamesPerRound = 0;
            for(int i =0; i< size/2;i++) {
                match2 = new MatchQueue::Match(cityIds[i], cityIds[j], *date);
                currentRound->matchQueue->enqueue(match2);
                scheduledGamesPerRound++;
                if(scheduledGamesPerRound == size/4)
                    date = addDays(date, 1);
                j--;
            }
            scheduledGamesPerRound = 0;
            date = addDays(date, 6);

            RoundList::Round *previousRound = currentRound;
            currentRound = currentRound->next;

            //Following rounds (1st leg)
            for(int l= 0; l< size-2;l++)
            {
                for(int i = 0; i<size-1; i++)
                {
                    if(!hasPlayedRound[i]){
                        j = whoDidTheyPlayAgainst(cityIds[i], previousRound);
                        j = indexOf(j, cityIds);
                        if(cityIds[j] == optionalTeam)
                            j = i;
                        j++;
                        if(j>size-1)
                            j = 0;
                        if(cityIds[i] == cityIds[j])
                            j=size-1;

                        if(l%2==0)
                            match2 = new MatchQueue::Match(cityIds[j], cityIds[i], *date);
                        else
                            match2 = new MatchQueue::Match(cityIds[i], cityIds[j], *date);
                        currentRound->matchQueue->enqueue(match2);
                        scheduledGamesPerRound++;
                        if(scheduledGamesPerRound==size/4)
                            date = addDays(date, 1);
                        hasPlayedRound[i] = true;
                        hasPlayedRound[j] = true;
                    }
                }
                for(int m = 0; m<hasPlayedRound.size(); m++)
                    hasPlayedRound[m] = false;
                previousRound = currentRound;
                currentRound = currentRound->next;
                scheduledGamesPerRound = 0;
                date = addDays(date, 6);
            }

            //2nd leg
            RoundList::Round *firstLegRound = roundList->head;

            for(int i=0; i<size-1; i++)
            {
                for(MatchQueue::Match *firstLegMatch=firstLegRound->matchQueue->head;
                        firstLegMatch!=NULL; firstLegMatch = firstLegMatch->next)
                {
                    match2 = new MatchQueue::Match(firstLegMatch->teamAway, firstLegMatch->teamHome, *date);
                    currentRound->matchQueue->enqueue(match2);
                    scheduledGamesPerRound++;
                    if(scheduledGamesPerRound==size/4)
                        date = addDays(date, 1);
                }
                firstLegRound = firstLegRound->next;
                currentRound = currentRound->next;
                scheduledGamesPerRound = 0;
                date = addDays(date, 6);
            }

            roundList->printCalendar();

        }
    }else if(command == "VIAJAR")
    {
        //cout << "ID_CIDADE_EM_QUE_O_JOGADOR_SE_ENCONTRA_ACTUALMENTE:";
        int city1;
        cin >> city1;
        //cout << "ID_CIDADE_QUE_O_JOGADOR_DESEJAR_VISITAR:";
        int city2;
        cin >> city2;

        CityList::City *source = cityList->getById(city1);
        CityList::City *destination = cityList->getById(city2);
        if(source == NULL)
        {
            cout << "ERRO! NÃO EXISTE CIDADE COM ID="<< city1 << endl;
        } else if(destination == NULL)
        {
            cout << "ERRO! NÃO EXISTE CIDADE COM ID="<< city2 << endl;
        } else{
            if(routeList->isEmpty() || city1 == city2)
            {
                cout << "ERRO! NÃO EXISTE ROTA DE "<<city1 <<" PARA " << city2 << endl;
            }
            else
            {
                int mostExp = routeList->printListMostExp(city1, city2);

                if(mostExp != -1) {
                    cout << "A ROTA MAIS CARA É " << mostExp << endl;
                    cout << "ID_ROTA:";
                    int routeId;
                    cin >> routeId;

                    RouteList::Route* route = routeList->getById(routeId);
                    if(route != NULL)
                    {
                        StopoverQueue::Stopover *stopover;
                        int lastCity = city1;
                        int totalRefuelCount = 0;
                        int maintenanceCount = 0;
                        cout << "ORIGEM_ROTA="<<city1 <<" DESTINO_ROTA="<<city2<<endl;
                        while(!route->stopoverQueue->isEmpty())
                        {
                            stopover = route->stopoverQueue->dequeue();

                            cout << "ORIGEM:" << lastCity << " DESTINO:" << stopover->idCity;
                            double distance = stopover->distance;
                            cout << " DISTANCIA:"<< distance;

                            int refuelCount = (int)distance / 150;
                            int refuelCost = refuelCount*50000;
                            cout << "KM ABASTECIMENTO: " << refuelCost;
                            totalRefuelCount+=refuelCount;

                            maintenanceCount = totalRefuelCount/3;
                            int maintenanceCost = maintenanceCount*30000;
                            cout << "MT MANUTENCAO: " << maintenanceCost <<"MT" << endl;

                            ofstream os("RotasPercorridas.txt", std::ios_base::app);
                            os << "ORIGEM:" << lastCity << " DESTINO:" << stopover->idCity
                                << " DISTANCIA:"<< distance << "KM ABASTECIMENTO: " << refuelCost
                                << "MT MANUTENCAO: " << maintenanceCost << "MT\n";
                            os.close();

                            lastCity = stopover->idCity;
                        }
                    } else{
                        cout << "ERRO! NÃO EXISTE ROTA DE ID=" << routeId << endl;
                    }
                }
                else
                    cout << "ERRO! NÃO EXISTE ROTA DE "<<city1 <<" PARA "<< city2 << endl;
            }
        }

    }else if(command == "ASSASSINATO_NO_MAPA_MUNDO")
    {
        int sourceId, destinationId;
        //cout << "CIDADE_ASSASSINATO:" ;
        cin >> sourceId;

        CityList::City *city1 = cityList->getById(sourceId);
        if(city1 == NULL)
            cout << "NÃO EXISTE CIDADE COM ID="<<sourceId << endl;
        else{
            //cout << "CIDADE_DOCUMENTO:";
            cin >> destinationId;
            if(sourceId == destinationId)
                cout << "O DOCUMENTO ENCONTRA-SE SEMPRE, NUMA CIDADE DIFERENTE"
                        " DA CIDADE ONDE OS POLÍCIAS ESTÃO ACTUALMENTE." << endl;
            else{
                CityList::City *city2 = cityList->getById(destinationId);
                if(city2 == NULL)
                    cout << "NÃO EXISTE CIDADE COM ID=" << destinationId << endl;
                else{
                    bool explored[cityList->size()];
                    for(int i=0; i< cityList->size(); i++)
                        explored[i] = false;

                    int randomCity = cityList->getRandomCity(destinationId);
                    ofstream os("Pista.txt");
                    os << randomCity;
                    os.close();

                    cityList->executeDijkstra(sourceId, destinationId);
                    int nrCities = readClue();
                    if(nrCities <1)
                        cout << "PISTA FALSA" << endl;
                    else
                    {
                        cout << "Deve percorrer " << nrCities << " cidades"  << endl;
                        int criminalCity = cityList->solveCrime(city2, explored, nrCities);
                        cout << "Criminoso apreendido na cidade " << criminalCity << endl;
                    }

                }
            }
        }

    }else
    {
        cout << "ERRO! COMANDO INVÁLIDO!" << endl;
    }
    listenForInput(cityList, routeList);
}

int main(){
    CityList* cityList = new CityList;
    RouteList* routeList = new RouteList;

    string username = "Utilizador";
    cout << "OLÁ CARO " << username <<  "!" << endl;
    cout << "BEM-VINDO AO EDA2JOGO :)" << endl;

    listenForInput(cityList, routeList);

    return 0;
}