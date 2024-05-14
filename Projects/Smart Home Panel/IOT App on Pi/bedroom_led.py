##
# Included Liberary
##
from google.cloud import firestore

#######################################

##
# Declared variables
##
light_1_curr = 0
light_1_prev = 0

lamp_1_curr = 0
lamp_1_prev = 0

##
# Initialize Firestore client
##
firestore_client = firestore.Client.from_service_account_json(
    "./autoome-b943c-ad9f82e890d2.json")


while (1):
    # Bedroom Ligt
    light_1_doc = firestore_client.collection("users") \
        .document("I3MMAZqPVkNcq3vmLtnzlZTPIoh2") \
        .collection("places") \
        .document("iRUPCuw9isLBrPXlNdmD") \
        .collection("rooms") \
        .document("53RVQ0PQpr6yle9o8Bih") \
        .collection("buttons") \
        .document("l4ATUD9Kd2I635F9Q80G")

    with open("light_1.txt", "r") as file:
        lamp_1_curr = file.read(1)
        print("File 1 : ", lamp_1_curr, " >  ", lamp_1_prev)

    # Get the document
    doc = light_1_doc.get()
    light_1_curr = doc.to_dict()
    light_1_curr = light_1_curr.get("boolVal")

    if ((light_1_curr != light_1_prev) and (lamp_1_curr == lamp_1_prev)):
        print("Light 1 : ", light_1_curr)
        with open("light_1.txt", "w") as file:
            file.write(str(light_1_curr))
        light_1_prev = light_1_curr
        lamp_1_curr = light_1_curr
        lamp_1_prev = lamp_1_curr
    elif ((lamp_1_prev != lamp_1_curr)):
        print("Lamp_1 : ", lamp_1_curr)
        light_1_doc.update({"boolVal": int(lamp_1_curr)})
        lamp_1_prev = lamp_1_curr
        light_1_curr = lamp_1_curr
        light_1_prev = light_1_curr
