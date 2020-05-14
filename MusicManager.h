//
// Created by User on 5/10/2020.
//

#ifndef WET1_STRUCTS_MUSICMANAGER_H
#define WET1_STRUCTS_MUSICMANAGER_H

#include "AvlTree.h"
#include "List.h"
#include "Artist.h"

// should these enums be exceptions?
typedef enum MMStatusType_t{
    MM_ALLOCATION_ERROR,
    MM_INVALID_INPUT,
    MM_FAILURE,
    MM_SUCCESS
}MMStatusType;


// MusicManager Class?
class MusicManager{
private:
    AvlTree<Artist,int> artists_tree;
    StreamList list_of_streams;
    int totalNumOfSongs;
public:
    MusicManager():artists_tree(*new AvlTree<Artist,int>), list_of_streams(*new(StreamList)),
                   totalNumOfSongs(0){};
    ~MusicManager();
    //MusicManager(const MusicManager& music_manager) = default;
    MusicManager& operator=(const MusicManager& music_manager) = default;

    //methods
    StreamList& GetListOfStreams() {return this->list_of_streams;};
    AvlTree<Artist,int>& GetArtistsTree() {return this->artists_tree;};


    MMStatusType MMAddArtist(int artistID, int numOfSongs);
    MMStatusType MMRemoveArtist(int artistID);
    MMStatusType MMAddToSongCount(int artistID, int songID);
    MMStatusType MMNumberOfStreams(int artistID, int songID, int* streams);
    MMStatusType MMgetRecommendedSongs( int numOfSongs, int* artists, int* songs);


};

MusicManager:: ~MusicManager(){
    delete &artists_tree;
    delete &list_of_streams;
}






MMStatusType MusicManager:: MMAddArtist( int artistID, int numOfSongs){
    // check ERRORS

    if(artistID<=0 ||  numOfSongs <=0){
        return MM_INVALID_INPUT;
    }

    AvlTree<Artist,int>& tree = this.GetArtistsTree();
    if(tree.findKeyAlreadyExists(artistID)){
        return MM_FAILURE;
    }
    Artist& artist_to_add = Artist(artistID, numOfSongs);


    StreamList& list = this->GetListOfStreams();
    StreamListNode* zero_streams_node = list.GetListFirstNode();
    // all songs now point to the "0" streams node
    for(int i = 0; i < artist_to_add.GetTotalNumOfSongs(); i++){
        artist_to_add.SetStreamsNumForSong(i, zero_streams_node);
    }

    // insert artist into songs
    tree.insert(artist_to_add, artistID);

    // insert pointer to the artist in "0" node AVLTree
    AvlTree<AvlTree<int,int>*,int>& node_tree = zero_streams_node->getNodeAvlTree();
    node_tree.insert((&(*(tree.getElementptr(artistID))->GetNumOfStreamsTree())),artistID);
    
    return MM_SUCCESS;

}





MMStatusType MusicManager::MMRemoveArtist(int artistID){
    // Check ERRORS

    if(artistID<=0 ){
        return MM_INVALID_INPUT;
    }

    AvlTree<Artist,int>& tree = this.GetArtistsTree();
    if(!(tree.findKeyAlreadyExists(artistID))){
        return MM_FAILURE;
    }
    Artist& artist = *(tree.getElementptr(artistID));

    // COMPLEXITY GOOD ENOUGH? NO NEED TO MAKE NULLS?
    for(int i = 0; i < artist.GetTotalNumOfSongs(); i++){
        StreamListNode* num_node = artist.GetSongNumOfStreamsNode(i);
        AvlTree<AvlTree<int,int>*,int>& node_tree = num_node->getNodeAvlTree();
        node_tree.remove(artistID);
        if(i != 0){
            if(node_tree.getFirst == nullptr){
                this->GetListOfStreams().removeNode(&num_node);
            }
        }
        artist.SetStreamsNumForSong(i,nullptr);
    }
    tree.remove(artistID);

    return MM_SUCCESS;

    // go to songs list
        // for every song
            // go to the node in the StreamList
            // if NULL - proceed to next song
            // remove the artist pointer from each node
                // remove the artist by key in the node's AVL Tree
            // change the pointer to NULL
        // delete the artist - remove from tree
        // delete the node if there are no more artists left

}





MMStatusType  MusicManager::MMAddToSongCount(int artistID, int songID){
    // Check ERRORS

    if(artistID<=0 ||  songID<0){
        return MM_INVALID_INPUT;
    }

    AvlTree<Artist,int>& tree = this.GetArtistsTree();
    if(tree.findKeyAlreadyExists(artistID)){
        return MM_FAILURE;
    }

    //get the tree, the artist and the list
    Artist& artist = *(tree.getElementptr(artistID));
    if(artist.GetTotalNumOfSongs()<=songID){
        return MM_INVALID_INPUT;
    }
    StreamList& list_of_streams = this->GetListOfStreams();
    AvlTree<int,int>* node_to_point_to = nullptr;
    StreamListNode* stream_list_node_to_point_to = nullptr;

    //change in the ArtistsTree
    AvlTree<AvlTree<int,int>,int>& num_of_streams_tree = artist.GetNumOfStreamsTree();
    StreamListNode* num_of_streams_list_node = artist.GetSongNumOfStreamsNode(songID);
    int songs_num_of_streams = *(num_of_streams_list_node)->GetNodeNumOfStreams();
    // remove song node
    AvlTree<int,int>& num_of_streams_tree_node = *(num_of_streams_tree.getElementptr(songs_num_of_streams));
    num_of_streams_tree_node.remove(songID);
    if(num_of_streams_tree_node.getFirst() == nullptr){
        num_of_streams_tree.remove(songs_num_of_streams);
    }
    // insert song to the next num_of_streams node
    if(num_of_streams_tree.findKeyAlreadyExists(songs_num_of_streams+1)){
        *(num_of_streams_tree.getElementptr(songs_num_of_streams+1))->insert(songID,songID);
    }
    else if{
        AvlTree<int,int>& new_songs_tree = AvlTree<int,int>();
        new_songs_tree.insert(songID,songID);
        num_of_streams_tree.insert(new_songs_tree,songs_num_of_streams+1);
        node_to_point_to = num_of_streams_tree.getElementptr(songs_num_of_streams+1);
    }

    /* OLD CODE
    AvlTree<int,int>& num_of_streams_tree_next_node = *(num_of_streams_tree.getNext()) // HOW TO USE GetNext?
    if(num_of_streams_tree_next_node.getKey() == ) {         // HOW TO USE GetKey?
        // get the next num_of_streams node
        // check if +1 exists
        // if yes - add the song to it
        node_to_point_to = &num_of_streams_tree_next_node;
    }
    else if{
        AvlTree<int,int>& new_songs_tree = AvlTree<int,int>();
        new_songs_tree.insert(songID,songID);
        num_of_streams_tree.insert(new_songs_tree, songs_num_of_streams+1);
        node_to_point_to = num_of_streams_tree.getElementptr(songs_num_of_streams+1);
    }

     */







    // change in the List
    StreamListNode* num_of_streams_list_next_node = num_of_streams_list_node->getNextNode();
    if(*(num_of_streams_list_next_node)->GetNodeNumOfStreams() == songs_num_of_streams+1){
        AvlTree<(AvlTree<int,int>)*,int>& num_of_streams_list_next_node_tree =
                *(num_of_streams_list_next_node)->getNodeAvlTree();
        num_of_streams_list_next_node_tree.insert(node_to_point_to,artistID);
        stream_list_node_to_point_to = num_of_streams_list_next_node;
    }
    else if{
        AvlTree<(AvlTree<int,int>)*,int>& new_node_tree = *(new AvlTree<(AvlTree<int,int>)*,int>());
        new_node_tree.insert(node_to_point_to,artistID);
        list_of_streams.insertNode(num_of_streams_list_node,new_node_tree,songs_num_of_streams+1);
        stream_list_node_to_point_to = num_of_streams_list_node->getNextNode();
    }
    // remove the artist's node from the original node's tree
    AvlTree<AvlTree<int,int>*,int>& num_of_streams_list_node_tree = *(num_of_streams_list_node)->getNodeAvlTree();
    num_of_streams_list_node_tree.remove(artistID);
    if(*(num_of_streams_list_node)->GetNodeNumOfStreams() != 0){
        if(num_of_streams_list_node_tree.getFirst() == nullptr){
            list_of_streams.removeNode(num_of_streams_list_node);
        }
    }

    // change in the songs array
    artist.SetStreamsNumForSong(songID,stream_list_node_to_point_to);

    return MM_SUCCESS;


    //get the tree, the artist and the list

    //change in the ArtistsTree
        // find the songs num_of_streams in the array
        // get the right num_of_streams tree
        // remove song node from it
            // check if it was the only song in the tree
                // if yes - remove the num_of_streams node
        // get the next num_of_streams node
            // check if +1 exists
                // if yes - add the song to it
                // if no - create a new num_of_streams node and add the song to it

    // change in the List
        // get the node in the StreamList from the songs array
        // get the node's AVLTree
        // check if next exists
            // if yes - get the pointer to the element (the tree)
            // if no - create one and add
// OLD PART
            // get the next StreamListNode
                // check if +1
                    // if yes - add the artist to it - pointer to the new "num_of_streams" node
                    // if no
                        // add a new +1 node to the StreamList
                        // add the artist to the node's tree - pointer the the new "num_of_streams" node
// END OF OLD PART
        // remove the artist's node from the tree
            // check if it wasn't the first node
                // check if it was the only song in the tree
                    // if yes - remove the node from the List

    // change in the songs array
        // change the pointer in the songID index to points to the new node in the StreamList

}





MMStatusType  MusicManager:: MMNumberOfStreams(int artistID, int songID, int* streams){
    // return ERRORS

    if(artistID<=0 ||  streams == nullptr){
        return MM_INVALID_INPUT;
    }

    AvlTree<Artist,int>& tree = this.GetArtistsTree();
    if(tree.findKeyAlreadyExists(artistID)){
        return MM_FAILURE;
    }

    Artist& artist= *(tree.getElementptr(artistID));
    if(artist.GetTotalNumOfSongs()<=songID){
        return MM_INVALID_INPUT;
    }
    StreamListNode* num_node = artist.GetSongNumOfStreams(songID);

    int num = num_node->GetNodeNumOfStreams();
    *streams = num;

    return MM_SUCCESS;

}




/*
MMStatusType MusicManager:: MMGetRecommendedSongs(void* DS, int numOfSongs, int* artists, int* songs){
    if(DS== nullptr || numOfSongs<=0){
        return MM_INVALID_INPUT;
    }
    if(numOfSongs<=0){
        return MM_ALLOCATION_ERROR;
    }
    DS.getRecommendedSongs( int numOfSongs, int* artists, int* songs);
}*/

MMStatusType MusicManager:: MMgetRecommendedSongs( int numOfSongs, int* artists, int* songs){
    if(numOfSongs>totalNumOfSongs){
        return MM_FAILURE;
    }
    StreamListNode* current_Node_of_hearings=list_of_streams.GetListLastNode();
    int count=0;;
    Artist* currentArtist;
    AvlTree<int,int>* songs_of_current_artist_with_num_streams;
    int* song;
    int current_ArtistId;
    while (count<numOfSongs){
        int numStreams=0;
        songs_of_current_artist_with_num_streams=*(((current_Node_of_hearings->getNodeAvlTree())).getFirst());

        while (count<numOfSongs &&songs_of_current_artist_with_num_streams){
            song=songs_of_current_artist_with_num_streams->getFirst();
            current_ArtistId=(current_Node_of_hearings->getNodeAvlTree()).getKey();
            while (song &&count<numOfSongs){
                artists[count]=current_ArtistId;
                songs[count]=(current_Node_of_hearings->getNodeAvlTree()).getKey();

                count++;
                song=songs_of_current_artist_with_num_streams->getNext();
            }

            songs_of_current_artist_with_num_streams=*((current_Node_of_hearings->getNodeAvlTree()).getNext());
        }
        current_Node_of_hearings=current_Node_of_hearings->getNextNode();
    }
    return MM_SUCCESS;

}






#endif //WET1_STRUCTS_MUSICMANAGER_H
