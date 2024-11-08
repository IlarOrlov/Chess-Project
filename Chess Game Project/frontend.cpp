#include "SFML/Graphics.hpp"
#include "PipeCommunicator.h"
#include "ChessGame.h"

// Function to get coordinates from chess notation
sf::Vector2f getCoordinatesFromChessNotation(const char* notation, const char* tileCoordinates[8][8], sf::Vector2f tileMouseClickCoordinates[8][8]) {
    // Map chess notation to array indices
    std::map<char, int> columnMapping = { {'a', 0}, {'b', 1}, {'c', 2}, {'d', 3}, {'e', 4}, {'f', 5}, {'g', 6}, {'h', 7} };

    // Extract column and row from chess notation
    char columnChar = notation[0];
    int row = notation[1] - '0';

    // Get column index from the map
    int column = columnMapping[columnChar];

    // Return the corresponding coordinates
    return tileMouseClickCoordinates[8 - row][column];
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "Chess Game", sf::Style::Titlebar | sf::Style::Close);

#pragma region Pipe Communicator Setting Up
    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = nullptr;

    // Create the pipe
    if (!CreatePipe(&PipeCommunicator::pipeHandles[0], &PipeCommunicator::pipeHandles[1], &saAttr, 0)) {
        std::cerr << "Error creating pipe." << std::endl;
        return 1;
    }
#pragma endregion

    // Create an instance of PipeCommunicator using the static getInstance method
    PipeCommunicator& communicator = PipeCommunicator::getInstance();
    ChessGame game;
    std::string source = "";
    std::string destination = "";
    bool isGameOver = false;

#pragma region General Preperation
    // Load font for text rendering
    sf::Font font;
    if (!font.loadFromFile("resources/Browood-Regular.ttf")) {
        std::cerr << "Error loading font file" << std::endl;
        return 1;
    }

    // Initialize chessboard sprite
    sf::Texture chessboardTexture;
    if (!chessboardTexture.loadFromFile("resources/chessboard.png")) {
        std::cerr << "Error loading chessboard texture" << std::endl;
        return 1;
    }
    sf::Sprite chessboardSprite(chessboardTexture);

    sf::Texture lettersTexture;
    lettersTexture.loadFromFile("resources/letters.png");
    sf::Sprite lettersSprite(lettersTexture);

    sf::Texture numbersTexture;
    numbersTexture.loadFromFile("resources/numbers.png");
    sf::Sprite numbersSprite(numbersTexture);

    // Set the size of the chessboard sprite
    float chessboardSize = 650.0f;
    float letterAndNumbersSize = 600.0f;
    float spriteScale = chessboardSize / (chessboardTexture.getSize().x);

    chessboardSprite.setScale(spriteScale, spriteScale);

    // Set the position of the chessboard sprite to the top center
    chessboardSprite.setPosition((window.getSize().x - chessboardSize) / 2.0f, 0.0f);

    // Set the scale and position of the letters sprite
    float lettersScale = letterAndNumbersSize / lettersTexture.getSize().x;
    float lettersX = chessboardSprite.getPosition().x + 20.0f;
    float lettersY = chessboardSprite.getPosition().y + chessboardSize;

    lettersSprite.setScale(lettersScale, lettersScale);
    lettersSprite.setPosition(lettersX, lettersY);

    // Set the scale and position of the numbers sprite
    float numbersScale = letterAndNumbersSize / numbersTexture.getSize().y;
    float numbersX = chessboardSprite.getPosition().x + chessboardSize;
    float numbersY = chessboardSprite.getPosition().y + 20.0f;

    numbersSprite.setScale(numbersScale, numbersScale);
    numbersSprite.setPosition(numbersX, numbersY);


    // Calculate the size of each tile based on the chessboard texture dimensions
    float tileWidth = chessboardSize / 8.0f;
    float tileHeight = chessboardSize / 8.0f;
#pragma endregion

#pragma region Coords And Positions
    // Define an array for tile coordinates
    const char* tileCoordinates[8][8] = {
        {"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"},
        {"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7"},
        {"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6"},
        {"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5"},
        {"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4"},
        {"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3"},
        {"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2"},
        {"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"}
    };

    sf::Vector2f tileMouseClickCoordinates[8][8];
    for (int column = 0; column < 8; ++column) {
        for (int row = 0; row < 8; ++row) {
            tileMouseClickCoordinates[row][column] = sf::Vector2f(
                chessboardSprite.getPosition().x + column * tileHeight + 7.5f,
                chessboardSprite.getPosition().y + row * tileWidth + 7.5f);
        }
    }
#pragma endregion

#pragma region Labels Seetings
    sf::Text movesLabel;
    movesLabel.setFont(font);
    movesLabel.setCharacterSize(30);
    movesLabel.setFillColor(sf::Color::Red);
    movesLabel.setPosition(10.0, chessboardSprite.getPosition().y + chessboardSize + 25.0f); // Adjust position as needed
    movesLabel.setString(""); // Set initial text

    sf::Text logLabel;
    logLabel.setFont(font);
    logLabel.setCharacterSize(30);
    logLabel.setFillColor(sf::Color::Red);
    logLabel.setPosition(10.0, chessboardSprite.getPosition().y + chessboardSize + 65.0f); // Adjust position as needed
    logLabel.setString(""); // Set initial text
    bool isThereIssue = false;

    sf::Text turnLabel;
    turnLabel.setFont(font);
    turnLabel.setCharacterSize(30);
    turnLabel.setFillColor(sf::Color::Red);
    turnLabel.setPosition(10.0, chessboardSprite.getPosition().y + chessboardSize + 105.0f); // Adjust position as needed
    turnLabel.setString("White's Turn"); // Set initial text
#pragma endregion

#pragma region Pieces Textures Load
    // Load pieces textures

    // Black Textures
    sf::Texture blackPawnTexture;
    sf::Texture blackRookTexture;
    sf::Texture blackBishopTexture;
    sf::Texture blackQueenTexture;
    sf::Texture blackKingTexture;
    sf::Texture blackKnightTexture;

    // White Textures
    sf::Texture whitePawnTexture;
    sf::Texture whiteRookTexture;
    sf::Texture whiteBishopTexture;
    sf::Texture whiteQueenTexture;
    sf::Texture whiteKingTexture;
    sf::Texture whiteKnightTexture;

    blackPawnTexture.loadFromFile("resources/BlackPawn.png");
    blackRookTexture.loadFromFile("resources/BlackRook.png");
    blackBishopTexture.loadFromFile("resources/BlackBishop.png");
    blackQueenTexture.loadFromFile("resources/BlackQueen.png");
    blackKingTexture.loadFromFile("resources/BlackKing.png");
    blackKnightTexture.loadFromFile("resources/BlackKnight.png");

    whitePawnTexture.loadFromFile("resources/WhitePawn.png");
    whiteRookTexture.loadFromFile("resources/WhiteRook.png");
    whiteBishopTexture.loadFromFile("resources/WhiteBishop.png");
    whiteQueenTexture.loadFromFile("resources/WhiteQueen.png");
    whiteKingTexture.loadFromFile("resources/WhiteKing.png");
    whiteKnightTexture.loadFromFile("resources/WhiteKnight.png");
#pragma endregion

#pragma region Sprites Initializing
    // Create instances of ChessPiece for each piece on the board
    std::vector<sf::Sprite> chessPieces;
    float pieceSize = chessboardSize / 9.5f;
    float scale = pieceSize / blackPawnTexture.getSize().x;

    // Iterate through rows and columns
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Piece* currentPiece = game.GetGameBoard().board[i][j];

            if (currentPiece != nullptr) {
                char pieceType = currentPiece->GetPiece();
                char pieceColor = currentPiece->GetColor();
                sf::Sprite pieceSprite;

                // Texture Determination
                switch (pieceType) {
                case 'P':
                    pieceSprite.setTexture((pieceColor == 'W') ? whitePawnTexture : blackPawnTexture);
                    break;
                case 'R':
                    pieceSprite.setTexture((pieceColor == 'W') ? whiteRookTexture : blackRookTexture);
                    break;
                case 'N':
                    pieceSprite.setTexture((pieceColor == 'W') ? whiteKnightTexture : blackKnightTexture);
                    break;
                case 'B':
                    pieceSprite.setTexture((pieceColor == 'W') ? whiteBishopTexture : blackBishopTexture);
                    break;
                case 'Q':
                    pieceSprite.setTexture((pieceColor == 'W') ? whiteQueenTexture : blackQueenTexture);
                    break;
                case 'K':
                    pieceSprite.setTexture((pieceColor == 'W') ? whiteKingTexture : blackKingTexture);
                    break;
                default:
                    // Adjust this case to your default piece texture
                    pieceSprite.setTexture(whiteBishopTexture);
                    break;
                }
                pieceSprite.setScale(scale, scale);
                pieceSprite.setPosition(tileMouseClickCoordinates[7 - i][j]);
                chessPieces.emplace_back(pieceSprite);
            }
        }
    }
#pragma endregion

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        while (!isGameOver) {
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }

                // Handle mouse clicks for moves
                if (event.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    std::cout << "Mouse Clicked at: " << mousePos.x << ", " << mousePos.y << std::endl;

                    // Check if the mouse click is within the chessboard bounds
                    if (mousePos.x >= chessboardSprite.getPosition().x &&
                        mousePos.x < chessboardSprite.getPosition().x + chessboardSize &&
                        mousePos.y >= chessboardSprite.getPosition().y &&
                        mousePos.y < chessboardSprite.getPosition().y + chessboardSize) {

                        // Map mouse position to chessboard coordinates
                        int column = static_cast<int>((mousePos.x - chessboardSprite.getPosition().x) / tileWidth);
                        int row = static_cast<int>((mousePos.y - chessboardSprite.getPosition().y) / tileHeight);
                        if (!source.empty() && !destination.empty() || source.empty() && destination.empty()) {
                            source = tileCoordinates[row][column];
                            destination = "";

                            movesLabel.setString("Move Selected From " + source);
                        }
                        else if (!source.empty() && destination.empty()) {
                            destination = tileCoordinates[row][column];

                            movesLabel.setString("Move Selected From " + source + " To " + destination);
                            communicator.writeToPipe(source + destination);
                            game.GetNextMove(game.GetGameBoard().board, communicator);

                            std::string protocolResult = communicator.readFromPipe();

                            isThereIssue = true;
                            switch (std::stoi(protocolResult)) {
                            case 0:
                                logLabel.setString("Normal move");
                                game.AlternateTurn();
                                break;
                            case 1:
                                logLabel.setString("Proper move, caused check on the opponent");
                                game.AlternateTurn();
                                break;
                            case 2:
                                logLabel.setString("Incorrect move, no tool of the current player in the source slot");
                                break;
                            case 3:
                                logLabel.setString("Incorrect move, tool of the current player in the target square");
                                break;
                            case 4:
                                logLabel.setString("Improper move, move will result in check on the current player");
                                break;
                            case 5:
                                logLabel.setString("Improper move, indexes of the slots are invalid");
                                break;
                            case 6:
                                logLabel.setString("Improper movement, illegal movement of a tool");
                                break;
                            case 7:
                                logLabel.setString("Improper move, source slot and destination slot are equals");
                                break;
                            case 8:
                                logLabel.setString("Correct move, checkmate was made");
                                isGameOver = true;
                                break;
                            case 9:
                                logLabel.setString("Correct move, stalemate was made - draw");
                                isGameOver = true;
                                break;
                            case 10:
                                logLabel.setString("Incorrect move, Player is in check");
                                break;
                            case 11:
                            {
                                logLabel.setString("Click On Queen, Rook, Bishop or Knight to choose pawn promotion");
                                source = destination;
                                destination = "";

                                sf::Event event2;
                                bool mousePressed = false;

                                while (true && window.isOpen()) {
                                    while (window.pollEvent(event2)) {
                                        if (event2.type == sf::Event::Closed) {
                                            window.close();
                                        }

                                        if (event2.type == sf::Event::MouseButtonPressed) {
                                            mousePressed = true;
                                        }
                                    }

                                    if (mousePressed) {
                                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                                        if (mousePos.x >= chessboardSprite.getPosition().x &&
                                            mousePos.x < chessboardSprite.getPosition().x + chessboardSize &&
                                            mousePos.y >= chessboardSprite.getPosition().y &&
                                            mousePos.y < chessboardSprite.getPosition().y + chessboardSize) {

                                            column = static_cast<int>((mousePos.x - chessboardSprite.getPosition().x) / tileWidth);
                                            row = static_cast<int>((mousePos.y - chessboardSprite.getPosition().y) / tileHeight);
                                            destination = tileCoordinates[row][column];
                                            communicator.writeToPipe(source + destination);

                                            game.PromotePawn(game.GetGameBoard().board, communicator);
                                            protocolResult = communicator.readFromPipe();

                                            if (protocolResult == "0") {
                                                logLabel.setString("Pawn Promoted ;)");
                                            }
                                            else {
                                                logLabel.setString("Click Is Improper, Promoting To Queen");
                                            }

                                            break;
                                        }
                                        else {
                                            logLabel.setString("Click Is Improper, Promoting To Queen");
                                            break;
                                        }
                                    }

                                    window.clear();
                                    window.draw(chessboardSprite);
                                    window.draw(lettersSprite);
                                    window.draw(numbersSprite);

                                    // Draw all chess pieces
                                    for (const auto& piece : chessPieces) {
                                        window.draw(piece);
                                    }

                                    window.draw(movesLabel);
                                    if (isThereIssue) {
                                        window.draw(logLabel);
                                    }

                                    window.display();
                                }
                                game.AlternateTurn();
                                break;
                            }
                            default:
                                isThereIssue = false;
                                logLabel.setString("");
                                break;
                            }

                            turnLabel.setString(game.GetPlayerTurn() == 'W' ? "White's Turn" : "Black's Turn");
                            chessPieces.clear();
                            #pragma region Redraw The Board
                            // Iterate through rows and columns
                            for (int i = 0; i < 8; ++i) {
                                for (int j = 0; j < 8; ++j) {
                                    Piece* currentPiece = game.GetGameBoard().board[i][j];

                                    if (currentPiece != nullptr) {
                                        char pieceType = currentPiece->GetPiece();
                                        char pieceColor = currentPiece->GetColor();
                                        sf::Sprite pieceSprite;

                                        // Texture Determination
                                        switch (pieceType) {
                                        case 'P':
                                            pieceSprite.setTexture((pieceColor == 'W') ? whitePawnTexture : blackPawnTexture);
                                            break;
                                        case 'R':
                                            pieceSprite.setTexture((pieceColor == 'W') ? whiteRookTexture : blackRookTexture);
                                            break;
                                        case 'N':
                                            pieceSprite.setTexture((pieceColor == 'W') ? whiteKnightTexture : blackKnightTexture);
                                            break;
                                        case 'B':
                                            pieceSprite.setTexture((pieceColor == 'W') ? whiteBishopTexture : blackBishopTexture);
                                            break;
                                        case 'Q':
                                            pieceSprite.setTexture((pieceColor == 'W') ? whiteQueenTexture : blackQueenTexture);
                                            break;
                                        case 'K':
                                            pieceSprite.setTexture((pieceColor == 'W') ? whiteKingTexture : blackKingTexture);
                                            break;
                                        default:
                                            // Adjust this case to your default piece texture
                                            pieceSprite.setTexture(whiteBishopTexture);
                                            break;
                                        }
                                        pieceSprite.setScale(scale, scale);
                                        pieceSprite.setPosition(tileMouseClickCoordinates[7 - i][j]);
                                        chessPieces.emplace_back(pieceSprite);
                                    }
                                }
                            }
                            #pragma endregion
                        }
                        std::cout << "Clicked on tile: " << tileCoordinates[row][column] << std::endl;
                    }
                    else {
                        // Display a message label for invalid coordinates
                        logLabel.setString("Unknown Position, Please Click On Valid Place!");
                        isThereIssue = true;
                    }
                }
            }

            window.clear();
            window.draw(chessboardSprite);
            window.draw(lettersSprite);
            window.draw(numbersSprite);

            // Draw all chess pieces
            for (const auto& piece : chessPieces) {
                window.draw(piece);
            }

            window.draw(movesLabel);
            window.draw(turnLabel);
            if (isThereIssue) {
                window.draw(logLabel);
            }

            window.display();
        }
    }

    return 0;
}